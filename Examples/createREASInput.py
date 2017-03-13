#! /usr/bin/env python2
# -*- coding: utf-8 -*-

"""mwOffline event information needed for CoREAS simulations. Geometry
is given in LOPES coordinate system for consumption by simtool. Each
output line contains:

  * Gt: seconds since 1/1/1970 UT
  * Azimuth: shower azimuth in degrees counted clockwise from (geographical)
             north
  * Zenith: shower zenith angle in degrees
  * X_c: geographical easting of shower core position in meters
  * Y_c: geographical northing of shower core position in meters
  * N_e, N_mu: reconstructed electron and muon count
  * E_0: reconstructed primary energy in eV
  * comment (starting with hash sign): event filename and number

"""

if __name__ == '__main__':
    import argparse
    import os

    import mwoffline.event as evt
    import mwoffline.detector as det
    import mwoffline.utilities as utl

    from pyik import time_conversion

    parser = argparse.ArgumentParser(
        description=("reads mwOffline event files and extracts information"
                     "needed for CoREAS simulations of the events"))
    parser.add_argument('input_files', metavar='event.root',
                        type=str, nargs='+',
                        help="mwOffline event files")

    print "# " + __doc__.strip().replace("\n", "\n# ") + "\n#"

    args = parser.parse_args()
    for fname in args.input_files:
        input_file = evt.EventFile(fname)

        event = evt.Event()
        input_file.SetEventBuffer(event)

        detector = det.DetectorSetup()
        input_file.ReadDetectorSetup(detector)
        det.Detector.GetInstance().InitFromFile(input_file)

        array = detector.GetArray()

        while input_file.ReadNextEvent() == input_file.eSuccess:
            data = {}
            array_data = event.GetArrayData()
            shower = array_data.GetShower()

            # Convert GPS second of event to Unix seconds
            gt = time_conversion.gps_to_utc(array_data.GetGPSSecond())

            # Convert azimuth angle to geographical CS: north = 0°, east = 90°
            shower_axis = utl.TVector3(shower.GetAxis())
            array.RotateFromArrayToGeographical(shower_axis)
            azimuth = ((90.0 - shower_axis.Phi() / utl.deg) + 360.0) % 360.0

            # Convert shower core to geographical CS
            shower_core = utl.TVector3(shower.GetCore())
            array.RotateFromArrayToGeographical(shower_core)

            print "%10i" % gt,
            print "%6.2f" % azimuth,
            print "%5.2f" % (shower.GetZenith() / utl.deg),
            print "%7.2f" % (shower_core.X() / utl.m),
            print "%7.2f" % (shower_core.Y() / utl.m),
            print "%11.1f" % shower.GetElectronSize(),
            print "%9.1f" % shower.GetMuonSize(),
            print "%8.2e" % (shower.GetEnergy() / utl.eV),
            print " # %s" % os.path.basename(fname),
            print "(event %i)" % event.GetEventId()
