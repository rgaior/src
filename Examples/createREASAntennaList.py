#! /usr/bin/env python2
# -*- coding: utf-8 -*-

"""Reads an mwOffline event file, extracts the antenna positions from
the detector description and prints it in the format needed for CoREAS
simulations.

"""

header = """# CROME antenna positions for %s
#
# AntennaPosition =	northcm		westcm		heightcm	id"""

if __name__ == '__main__':
    import argparse

    import os

    import mwoffline.event as evt
    import mwoffline.detector as det
    import mwoffline.utilities as utl

    parser = argparse.ArgumentParser(
        description=("reads an mwOffline event file and extracts antenna"
                     "positions needed for CoREAS simulations"))
    parser.add_argument('input_file', metavar='event.root',
                        type=str,
                        help="mwOffline event file")

    args = parser.parse_args()

    input_file = evt.EventFile(args.input_file)

    event = evt.Event()
    input_file.SetEventBuffer(event)

    detector = det.DetectorSetup()
    input_file.ReadDetectorSetup(detector)
    det.Detector.GetInstance().InitFromFile(input_file)

    array = detector.GetArray()

    print header % os.path.basename(args.input_file)
    for antenna in detector.GetAntennas():
        antenna_id = antenna.GetId()

        # Convert antenna position to geographical coordinates
        antenna_position = utl.TVector3(antenna.GetPosition())
        array.RotateFromArrayToGeographical(antenna_position)

        print "\t".join(["AntennaPosition =",
                         "%.1f" % (100 * antenna_position.Y()),
                         "%.1f" % (100 * -antenna_position.X()),
                         "%.1f" % (100 * (antenna_position.Z() + 110.1)),
                         "crome%i" % antenna_id])
