#! /usr/bin/env python2
# -*- coding: utf-8 -*-

import sys
from math import atan2, acos
from collections import namedtuple
from itertools import cycle

import numpy as np
from numpy import pi, sqrt, sin, cos
from matplotlib import pyplot, rc, patches
from matplotlib.ticker import FuncFormatter

from mwoffline.event import Event, EventFile
from mwoffline.detector import DetectorSetup
from mwoffline import utilities as utl


if len(sys.argv) < 5:
    print "Usage:", sys.argv[0], "Tree.root EventId AntennaId",
    print "Channel [Channel ...]"
    sys.exit(1)

filename = sys.argv[1]
event_id = int(sys.argv[2])
antenna_id = int(sys.argv[3])
channel_ids = map(int, sys.argv[4:])

Shower = namedtuple("Shower", "xc yc theta phi e0")

powerToDBm = np.vectorize(utl.powerToDBm)

def coordsAt(r, shower, antenna):
    phi, theta = shower.GetAxis().Phi(), shower.GetAxis().Theta()
    xc, yc = shower.GetCore().X(), shower.GetCore().Y()

    n = np.array((r * sin(theta) * cos(phi),
                  r * sin(theta) * sin(phi),
                  r * cos(theta)))
    x, y, z = np.array((xc, yc, 0)) + n
    x = x - antenna.GetPosition().X()
    y = y - antenna.GetPosition().Y()

    r0 = sqrt(x**2 + y**2 + z**2)
    theta0 = acos(z / sqrt(x**2 + y**2 + z**2))
    phi0 = atan2(y, x)

    return r0, theta0 * 180. / pi, phi0 * 180. / pi


def getEvent(filename, event_id):
    inputFile = EventFile(filename)
    event = Event()
    detector = DetectorSetup()

    inputFile.SetEventBuffer(event)
    inputFile.ReadEvent(event_id)
    inputFile.ReadDetectorSetup(detector)

    return detector, event


def getTraces(detector, event, antenna_id, channel_ids):
    antenna = detector.GetAntenna(antenna_id)
    antenna_data = event.GetAntennaData(antenna_id)

    times, noise_levels, det_thresholds, traces = [], [], [], []
    for channel_id in channel_ids:
        channel = antenna.GetChannel(channel_id)
        channel_data = antenna_data.GetChannelData(channel_id)

        trace = np.array(channel_data.GetSimulatedPowerTrace())
        noise_temperature = 80. * utl.kelvin
        dish_radius = 3.35 / 2. * utl.m
        dish_efficiency = 0.7
        bandwidth = 570e6 * utl.Hz
        integration_time = 4e-9 * utl.second

        dish_effective_area = pi * dish_radius**2 * dish_efficiency
        noise_level = utl.kBoltzmann * noise_temperature * bandwidth
        det_threshold = (noise_level + utl.kBoltzmann * noise_temperature /
                         (dish_effective_area * sqrt(bandwidth * integration_time)))

        dt = channel.GetTraceBinWidth()
        t0 = channel.GetTraceStartTime()
        time = t0 + np.arange(trace.size) * dt

        traces.append(trace)
        noise_levels.append(noise_level)
        det_thresholds.append(det_threshold)
        times.append(time)

    return times, traces, noise_levels, det_thresholds


def getChannelPointings(detector, antenna_id, channel_ids):
    antenna = detector.GetAntenna(antenna_id)
    thetas, phis, widths = [], [], []
    for channel_id in channel_ids:
        channel = antenna.GetChannel(channel_id)
        thetas.append(channel.GetLocalElevation() / utl.degree)
        phis.append(channel.GetLocalAzimuth() / utl.degree)
        widths.append(channel.GetBeamWidth() / utl.degree)

    return thetas, phis, widths


detector, event = getEvent(filename, event_id)
shower = event.GetArrayData().GetShower()
antenna = detector.GetAntenna(antenna_id)
times, traces, noise_levels, det_thresholds = getTraces(detector, event,
                                                        antenna_id, channel_ids)
thetas, phis, widths = getChannelPointings(detector, antenna_id, channel_ids)

# Create the plot
rc('text', usetex=True)
rc('font',**{'family':'sans-serif',
             'sans-serif':['Helvetica'],
             'size':8.0})

figure = pyplot.figure(figsize=(3.2, 2.4))
topplot = figure.add_subplot(1, 1, 1)

colors = 'gkrbcm'
if len(colors) < len(channel_ids):
    print "WARNING: Not enough colors for requested number of channels."
    print "         Colors will not match."
colors = cycle(colors[:len(channel_ids)])


for time, noise_level, det_threshold, trace in zip(times, noise_levels,
                                                   det_thresholds, traces):
    topplot.plot(time / utl.ns,
                 (powerToDBm(noise_level + trace) -
                  powerToDBm(noise_level)),
                 linestyle='steps',
                 color=colors.next())

    topplot.axhline(powerToDBm(det_threshold) - powerToDBm(noise_level),
                    linestyle='--', color='k')

topplot.set_xlim(0 * utl.ns, 80 * utl.ns)

topplot.yaxis.set_major_formatter(
    FuncFormatter(lambda x, y: r"\SI{%g}{dB}" % x))
topplot.xaxis.set_major_formatter(
    FuncFormatter(lambda x, y: r"\SI{%g}{\nano\second}" % x))
topplot.set_xlabel(r"Time after impact")
topplot.set_ylabel(r"Power relative to \SI{80}{K} noise")


inset = figure.add_axes([.525, .475, .4, .4], projection='polar')

shower_rs, shower_thetas, shower_phis = \
    zip(*[coordsAt(r, shower, antenna)
          for r in [0., 1000., 15.e3]])
inset.plot(np.array(shower_phis) / 180. * pi,
           shower_thetas, '.r-', lw=1., ms=4.,
           zorder=30)
for r0, theta0, phi0 in zip(shower_rs, shower_thetas, shower_phis):
    inset.annotate(r"  \SI{%.1f}{km}" % (r0 / 1000.), (phi0 / 180. * pi, theta0),
                   va='top', ha='right', zorder=20, size=6,
                   bbox={'facecolor':(1.,1.,1.,0.7), 'edgecolor':'none'})

for theta, phi, width in zip(thetas, phis, widths):
    if theta == 90.0:
        angles = np.linspace(0, 2 * pi, 360.)
        inset.plot(angles, [width / 2.] * angles.size, colors.next(), lw=1.)
    else:
        fov = patches.Ellipse((phi * pi / 180., (90. - theta)),
                              (width * pi / 180. /
                               sin((90. - theta) * pi / 180.)), width,
                              fc='w', ec=colors.next())
        inset.add_patch(fov)

inset.set_thetagrids([0., 90., 180., 270.], frac=1.2)
inset.set_rgrids([3, 6, 9], angle=45, rpad=0.05,
                 bbox={'facecolor':(1.,1.,1.,0.7), 'edgecolor':'none'})
inset.set_rmax(10)
inset.yaxis.set_major_formatter(
    FuncFormatter(lambda x, y: r"\ang{%g}" % x))
inset.xaxis.set_major_formatter(
    FuncFormatter(lambda x, y: r"\ang{%g}" % (x * 180. / pi)))

for label in inset.get_xticklabels() + inset.get_yticklabels():
    label.set_fontsize(6)

pyplot.tight_layout()

pyplot.savefig("Event-Simulation.pdf", bbox_inches='tight')
#pyplot.savefig("Event-Simulation.eps", bbox_inches='tight')
#pyplot.savefig("Event-Simulation.png", bbox_inches='tight')

pyplot.show()
