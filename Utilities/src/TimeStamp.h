#include <string>
#include <ostream>


struct tm;

namespace utl {

  class TimeInterval;


  class TimeStamp {

  public:
    TimeStamp() : fGPSSecond(0), fGPSNanoSecond(0) { }

    /// Construct from GPS second and nanosecond
    TimeStamp(const unsigned int sec, const unsigned int nsec = 0) :
      fGPSSecond(sec), fGPSNanoSecond(nsec) { }

    TimeStamp(const int sec, const int nsec = 0)
    { SetNormalized(sec, nsec); }

    /// Construct from UTC date and time
    /*! <b> WARNING: this constructor does not (currently) carry out a complete
      set of checks that the input numbers are all within sensible ranges </b>
    */
    TimeStamp(const unsigned int utcYear,
              const unsigned int utcMonth,
              const unsigned int utcDay,
              const unsigned int utcHour = 0,
              const unsigned int utcMinute = 0,
              const unsigned int utcSecond = 0,
              const unsigned int utcNanosecond = 0)
    { SetUTCTime(utcYear, utcMonth, utcDay,
                 utcHour, utcMinute, utcSecond, utcNanosecond); }

    TimeStamp(const TimeStamp& ts) :
      fGPSSecond(ts.fGPSSecond), fGPSNanoSecond(ts.fGPSNanoSecond) { }

    TimeStamp& operator=(const TimeStamp& ts)
    { fGPSSecond = ts.fGPSSecond; fGPSNanoSecond = ts.fGPSNanoSecond;
      return *this; }

    // --------
    // Setters
    // --------

    /// Set GPS second and (optionally) nanosecond
    void SetGPSTime(const unsigned int sec, const unsigned int nsec = 0)
    { fGPSSecond = sec; fGPSNanoSecond = nsec; }

    // -------
    // Getters
    // -------

    /// GPS second
    unsigned int GetGPSSecond() const { return fGPSSecond; }

    /// GPS nanosecond
    unsigned int GetGPSNanoSecond() const { return fGPSNanoSecond; }

    /// Auger-formatted UTC date and time string 
    std::string GetUTCDateTime() const;

    /// XML-formatted date and time string
    std::string GetXMLDateTime() const;

    /// Return UTC date and time as unsigned ints passed in argument list
    /*! note that months are numbered beginning with January = 1 (not 0) */
    void GetUTCDateTime(unsigned int& utcYear,
                        unsigned int& utcMonth,
                        unsigned int& utcDay,
                        unsigned int& utcHour,
                        unsigned int& utcMinute,
                        unsigned int& utcSecond,
                        unsigned int& utcNanosecond) const;

    /// Get the year for UTC
    unsigned int GetYear() const;

    /// Get the month for UTC.  (range of months is 1-12)
    unsigned int GetMonth() const;

    /// Get the day for UTC
    unsigned int GetDay() const;

    /// Get the hour for UTC
    unsigned int GetHour() const;

    /// Get the minute for UTC
    unsigned int GetMinute() const;

    /// Get the second for UTC
    unsigned int GetSecond() const;

    /// Get the nanosecond for UTC
    unsigned int GetNanoSecond() const { return fGPSNanoSecond; }

    // Formatted date and time string for local time zone
    std::string GetLocalDateTime() const;

    /// Modified Julian Date
    double GetMJD() const;



  private:

    // with this we will have a y2k problem in approx. 136 years -
    // or for some operations in 68 years - from 1980 (GPS epoch)
    unsigned int fGPSSecond;
    unsigned int fGPSNanoSecond;

    /// Take care of positive nanosecond and GPS epoch
    void SetNormalized(int sec, int nsec);

    /// Puts the tm structure into format defined in conventions document
    std::string GetFormattedFromTm(const tm* const timeStruct) const;

    /// Get seconds after Unix Epoch
    inline time_t GetUnixSecond() const;
    
    /// Calculate leap seconds since GPS Epoch.
    inline unsigned int GetLeapSeconds() const;

    /// Check if the time sits directly on a leap second
    inline bool IsLeapBoundary() const;

    /// Set UTC date and time
    /*! This method is meant as a convenience for tasks such as 
        converting certain time stamps (for example station 
	or telescope commission times) from human-readable UTC into 
	a TimeStamp.  In other words, one might use this method to 
	convert time stamps stored in an external file where it may
	be more convenient to store UTC than GPS second.
	<b> WARNING: this method does not (currently) carry out a complete
	set of checks that the input numbers are all within sensible ranges </b>
    */
    void SetUTCTime(const unsigned int year, 
		    const unsigned int month,
		    const unsigned int day,
		    const unsigned int hour,
		    const unsigned int minute = 0,
		    const unsigned int second = 0,
		    const unsigned int nanosecond = 0);

  };


} // namespace utl


// Configure (x)emacs for this file ...
// Local Variables:
// mode: c++
// End:
