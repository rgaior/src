#include <sstream>
#include <iomanip>
#include <time.h>

#include "AugerUnits.h"
#include "TimeStamp.h"

using namespace std;
using namespace utl;


static const char* monthAbbr[13] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
				     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",					  
				     "#!#" };

const TimeStamp kMaxTimeStamp(2147483647U, 999999999U);

const int kSecPerDay = 24*3600;

// Initial offset between GPS and UTC times
// (1 January 1970 to 6 January 1980 = 10 years, 5 days, 2 leap-days)
const unsigned long int kUTCGPSOffset0 = (10*365 + 7)*kSecPerDay;


// MJD for GPS epoch (6 January 1980)
const unsigned long int kMJDOffset0 = 44244;


// Table of GPS seconds when leap second occured.
// THIS HAS TO BE EDITED BY HAND WHENEVER A NEW LEAP SECOND
// OCCURS.  Maybe there is a better way... D.V.: What about putting it
// in an XML file and read it in at singleton runtime initialization so
// that the code does not need recompilation?
//
// Table contains pairs of (GPS second when leap occurred, correction to apply)
//
const unsigned int kNumLeaps = 16;
const unsigned int kLeapSeconds[kNumLeaps][2] = {
  //
  // (GPS epoch + years + leap days + Jan-Jul)*kSecPerDay + leapSeconds
  //                                           |
  { (361 +   0*365 +   0 + 181)*kSecPerDay +  0, +1  },    // 1 JUL 1981
  { (361 +   1*365 +   0 + 181)*kSecPerDay +  1, +2  },    // 1 JUL 1982
  { (361 +   2*365 +   0 + 181)*kSecPerDay +  2, +3  },    // 1 JUL 1983
  { (361 +   4*365 +   1 + 181)*kSecPerDay +  3, +4  },    // 1 JUL 1985
  { (361 +   7*365 +   1      )*kSecPerDay +  4, +5  },    // 1 JAN 1988
  { (361 +   9*365 +   2      )*kSecPerDay +  5, +6  },    // 1 JAN 1990
  { (361 +  10*365 +   2      )*kSecPerDay +  6, +7  },    // 1 JAN 1991
  { (361 +  11*365 +   3 + 181)*kSecPerDay +  7, +8  },    // 1 JUL 1992
  { (361 +  12*365 +   3 + 181)*kSecPerDay +  8, +9  },    // 1 JUL 1993
  { (361 +  13*365 +   3 + 181)*kSecPerDay +  9, +10 },    // 1 JUL 1994
  { (361 +  15*365 +   3      )*kSecPerDay + 10, +11 },    // 1 JAN 1996
  { (361 +  16*365 +   4 + 181)*kSecPerDay + 11, +12 },    // 1 JUL 1997
  { (361 +  18*365 +   4      )*kSecPerDay + 12, +13 },    // 1 JAN 1999
  // DV: 2000 IS a leap year since it is divisible by 400,
  //     ie leap years here are 2000 and 2004 -> leap days = 6
  { (361 +  25*365 +   6      )*kSecPerDay + 13, +14 },    // 1 JAN 2006
  { (361 +  28*365 +   7      )*kSecPerDay + 14, +15 },    // 1 JAN 2009
  { (361 +  31*365 +   8 + 181)*kSecPerDay + 15, +16 }     // 1 JUL 2012
};



/*! Returns seconds since Unix epoch (1 Jan, 1970).  Accounts for leap
    seconds relative to GPS time. This method is kept private to avoid
    suggesting that people should directly use Unix time_t themselves
    to compute time intervals.  POSIX standard does not handle leap
    seconds, so time_t intervals subtracted over a leap second do not
    give the correct number of seconds elapsed.
*/
inline
time_t
TimeStamp::GetUnixSecond()
  const
{
  return time_t(fGPSSecond + kUTCGPSOffset0 - GetLeapSeconds());
}


void
TimeStamp::SetNormalized(int sec, int nsec)
{

  // normalize
  const int sperns = int(second / nanosecond);
  if (nsec < 0) {
    --sec;
    nsec += sperns;
  } else if (nsec >= sperns) {
    ++sec;
    nsec -= sperns;
  }

  // must be within the GPS epoch
  if (sec >= 0) {
    fGPSSecond = sec;
    fGPSNanoSecond = nsec;
  } else {
    ostringstream err;
    err << "operation for "
           "sec = " << sec << " nsec = " << nsec << " "
           "results in time before GPS Epoch";
    fGPSSecond = fGPSNanoSecond = 0;
  }
}


string
TimeStamp::GetFormattedFromTm(const tm* const timeStruct)
  const
{
  // Format the time and date, following the format defined in the Auger 
  // conventions document.

  const int tmmon = timeStruct->tm_mon;
  int month;
  if (0 <= tmmon && tmmon <= 11)
    month = tmmon;
  else {
    month = 12;
  }

  ostringstream timeDateStream;
  timeDateStream << setw(2) << setfill('0') << timeStruct->tm_hour << ":"
		 << setw(2) << setfill('0') << timeStruct->tm_min  << ":"
		 << setw(2) << setfill('0') << timeStruct->tm_sec  << " "
		 << setw(2) << setfill('0') << timeStruct->tm_mday << " "
		 <<                            monthAbbr[month]    << " "
		 <<                            timeStruct->tm_year + 1900;

  return timeDateStream.str();
}


/*! Returns the UTC date and time formatted in accordance
    with Auger conventions document.
 */
string
TimeStamp::GetUTCDateTime()
  const
{
  const time_t theTime = GetUnixSecond();
  tm* const gmt = gmtime(&theTime);
  
  // Handle special case where time is a leap second
  if (IsLeapBoundary())
    gmt->tm_sec = 60;

  return GetFormattedFromTm(gmt);
}


string
TimeStamp::GetXMLDateTime()
  const
{
  unsigned int utcYear;
  unsigned int utcMonth;
  unsigned int utcDay;
  unsigned int utcHour;
  unsigned int utcMinute;
  unsigned int utcSecond;
  unsigned int utcNanosecond;
  GetUTCDateTime(utcYear, utcMonth, utcDay,
                 utcHour, utcMinute, utcSecond, utcNanosecond);

  ostringstream xml;
  xml << setw(4) << utcYear << '-'
      << setw(2) << setfill('0') << utcMonth << '-'
      << setw(2) << setfill('0') << utcDay << 'T'
      << setw(2) << setfill('0') << utcHour << ':'
      << setw(2) << setfill('0') << utcMinute << ':'
      << setw(2) << setfill('0') << utcSecond << 'Z';

  return xml.str();
}


void
TimeStamp::GetUTCDateTime(unsigned int& utcYear,
                          unsigned int& utcMonth,
                          unsigned int& utcDay,
                          unsigned int& utcHour,
                          unsigned int& utcMinute,
                          unsigned int& utcSecond,
                          unsigned int& utcNanosecond)
  const
{
  const time_t theTime = GetUnixSecond();
  tm* const gmt = gmtime(&theTime);

  // Handle special case where time is a leap second
  if (IsLeapBoundary())
    gmt->tm_sec = 60;

  utcYear = gmt->tm_year+1900;
  utcMonth = gmt->tm_mon + 1;
  utcDay = gmt->tm_mday;
  utcHour = gmt->tm_hour;
  utcMinute = gmt->tm_min;
  utcSecond = gmt->tm_sec;
  utcNanosecond = fGPSNanoSecond;
}


unsigned int
TimeStamp::GetYear()
  const
{
  const time_t theTime = GetUnixSecond();
  return gmtime(&theTime)->tm_year + 1900;
}


unsigned int
TimeStamp::GetMonth()
  const
{
  const time_t theTime = GetUnixSecond();
  return gmtime(&theTime)->tm_mon + 1;
}


unsigned int
TimeStamp::GetDay()
  const
{
  const time_t theTime = GetUnixSecond();
  return gmtime(&theTime)->tm_mday;
}


unsigned int
TimeStamp::GetHour()
  const
{
  const time_t theTime = GetUnixSecond();
  return gmtime(&theTime)->tm_hour;
}


unsigned int
TimeStamp::GetMinute()
  const
{
  const time_t theTime = GetUnixSecond();
  return gmtime(&theTime)->tm_min;
}


unsigned int
TimeStamp::GetSecond()
  const
{
  // Handle special case where time is a leap second
  if (IsLeapBoundary())
    return 60;

  const time_t theTime = GetUnixSecond();
  return gmtime(&theTime)->tm_sec;
}


/*! Returns date and time in local time zone formatted in accordance
    with Auger conventions document. Reports offset from GMT.
 */
string
TimeStamp::GetLocalDateTime()
  const
{
  const time_t theTime = GetUnixSecond();
  tm* const lt = localtime(&theTime);

  // Handle special case where time is a leap second
  if (IsLeapBoundary())
    lt->tm_sec = 60;

  tm* const gm = gmtime(&theTime);
  const time_t tdiffgm = mktime(gm);
  const time_t tdifflt = mktime(lt);
  const time_t timeZone = tdiffgm - tdifflt;

  stringstream timeZoneString;
  timeZoneString << " (" << (timeZone >= 0 ? "+" : "-")
		 << timeZone/3600 << "h GMT)";

  return GetFormattedFromTm(lt) + timeZoneString.str();
}


double
TimeStamp::GetMJD()
  const
{
  return double(fGPSSecond - GetLeapSeconds())/kSecPerDay + kMJDOffset0;
}


inline
unsigned int
TimeStamp::GetLeapSeconds()
  const
{
  for (int i = kNumLeaps-1; i >= 0; --i)
    if (fGPSSecond >= kLeapSeconds[i][0])
      return kLeapSeconds[i][1];
  return 0;
}


inline
bool
TimeStamp::IsLeapBoundary()
  const
{
  for (int i = kNumLeaps-1; i >= 0; --i) {
    const unsigned int leapSec = kLeapSeconds[i][0];
    if (fGPSSecond > leapSec)
      return false;
    if (fGPSSecond == leapSec) 
      return true;
  }
  return false;
}


void
TimeStamp::SetUTCTime(const unsigned int utcYear, 
                      const unsigned int utcMonth,
                      const unsigned int utcDay,
                      const unsigned int utcHour,
                      const unsigned int utcMinute,
                      const unsigned int utcSecond,
                      const unsigned int utcNanosecond)
{
  // This is not a complete set of checks !
  if (utcMonth < 1 || utcMonth > 12) {
    ostringstream err;
    err << "month " << utcMonth << " not in range 1-12";
  }
  if (utcDay < 1 || utcDay > 31) {
    ostringstream err;
    err << "day " << utcDay << " not in range 1-31";
  }
  if (utcYear < 1900) {
    ostringstream err;
    err << "year " << utcYear << " less than 1900";
  }
  if (utcMinute > 59) {
    ostringstream err;
    err << "minute " << utcMinute << " not in range 0-59";
  }
  if (utcSecond > 60) {
    ostringstream err;
    err << "second " << utcSecond << " not in range 0-60";
  }
  if (utcHour > 23) {
    ostringstream err;
    err << "hour " << utcHour << " not in range 0-23";
  }

  tm timeStruct;
  timeStruct.tm_sec   = utcSecond;
  timeStruct.tm_min   = utcMinute;
  timeStruct.tm_hour  = utcHour;
  timeStruct.tm_mday  = utcDay;
  timeStruct.tm_mon   = utcMonth - 1;  // tm_mon in range 0-11
  timeStruct.tm_year  = utcYear - 1900;
  timeStruct.tm_wday  = 1;             // computed by mktime
  timeStruct.tm_yday  = 1;             // computed by mktime
  timeStruct.tm_isdst = 0;             // no DST corrections
                                       // (we correct back to GMT below)

  // calculate unix seconds at GMT
  // const time_t theTime = mktime(&timeStruct) - timezone;

  // find time difference between local and gmt.  
  // this looks a bit stupid, but need something that
  // works on all ctime libraries 
  //  
  const time_t t1 = mktime(&timeStruct);
  tm* const gm = gmtime(&t1);
  const time_t tdiffgm = mktime(gm);
  tm* const lt = localtime(&t1);
  const time_t tdifflt = mktime(lt);
 
  const time_t theTime = mktime(&timeStruct) - (tdiffgm - tdifflt);

  if (theTime < time_t(kUTCGPSOffset0)) {
    const string err = "Initializing TimeStamp to a time before the GPS Epoch";
  }

  fGPSSecond = theTime - kUTCGPSOffset0;        // first set without leap seconds
  const unsigned int leap1 = GetLeapSeconds();  // later corrected for leap seconds
  fGPSSecond += leap1;
  const unsigned int leap2 = GetLeapSeconds();

  // leap second lookup correction (since our lookup time
  // did not include leap seconds)  
  if ((leap2 == (leap1 + 1)) && utcSecond != 60)
    ++fGPSSecond;

  fGPSNanoSecond = utcNanosecond;
}
