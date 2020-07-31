# -*- coding: utf-8 -*-

"""
config_webrx: configuration options for OpenWebRX

    This file is part of OpenWebRX,
    an open-source SDR receiver software with a web UI.
    Copyright (c) 2013-2015 by Andras Retzler <randras@sdr.hu>
    Copyright (c) 2019-2020 by Jakob Ketterl <dd5jfk@darc.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    In addition, as a special exception, the copyright holders
    state that config_rtl.py and config_webrx.py are not part of the
    Corresponding Source defined in GNU AGPL version 3 section 1.

    (It means that you do not have to redistribute config_rtl.py and
    config_webrx.py if you make any changes to these two configuration files,
    and use them for running your web service with OpenWebRX.)

    portions inspired by http://gephi.michalnovak.eu/config_webrx.py    

"""

# configuration version. please only modify if you're able to perform the associated migration steps.
version = 2

# NOTE: you can find additional information about configuring OpenWebRX in the Wiki:
# https://github.com/jketterl/openwebrx/wiki/Configuration-guide

# ==== Server settings ====
web_port = 8073
max_clients = 20

# ==== Web GUI configuration ====
receiver_name = "KU2Y"
receiver_location = "Philadelphia, PA"
receiver_asl = 200
receiver_admin = "ku2y@amsat.org"
receiver_gps = (40.0376, -75.3492)
receiver_gps = {"lat": 40.0376, "lon": -75.3492}
photo_title = "ARISS"
photo_desc = """
You can add your own background photo and receiver information.<br />
Receiver is operated by: <a href="mailto:ku2y@amsat.org">ku2y@amsat.org</a><br/>
Device: RTL-SDR<br />
Antenna: <br />
Website: <a href="http://ariss-radio:8073" target="_blank">http://ariss-radio:8073</a>
"""

# ==== DSP/RX settings ====
fft_fps = 9
fft_size = 4096  # Should be power of 2
fft_voverlap_factor = (
    0.3  # If fft_voverlap_factor is above 0, multiple FFTs will be used for creating a line on the diagram.
)

audio_compression = "adpcm"  # valid values: "adpcm", "none"
fft_compression = "adpcm"  # valid values: "adpcm", "none"

digimodes_enable = True  # Decoding digimodes come with higher CPU usage.
digimodes_fft_size = 1024

# determines the quality, and thus the cpu usage, for the ambe codec used by digital voice modes
# if you're running on a Raspi (up to 3B+) you'll want to leave this on 1
digital_voice_unvoiced_quality = 1
# enables lookup of DMR ids using the radioid api
digital_voice_dmr_id_lookup = True

"""
Note: if you experience audio underruns while CPU usage is 100%, you can: 
- decrease `samp_rate`,
- set `fft_voverlap_factor` to 0,
- decrease `fft_fps` and `fft_size`,
- limit the number of users by decreasing `max_clients`.
"""

# ==== I/Q sources ====
# (Uncomment the appropriate by removing # characters at the beginning of the corresponding lines.)

###############################################################################
# Is my SDR hardware supported?                                               #
# Check here: https://github.com/jketterl/openwebrx/wiki/Supported-Hardware   #
###############################################################################

# Currently supported types of sdr receivers:
# "rtl_sdr", "rtl_sdr_soapy", "sdrplay", "hackrf", "airspy", "airspyhf", "fifi_sdr",
# "perseussdr", "lime_sdr", "pluto_sdr", "soapy_remote"
#
# In order to use rtl_sdr, you will need to install librtlsdr-dev and the connector.
# In order to use sdrplay, airspy or airspyhf, you will need to install soapysdr, the corresponding driver, and the
# connector.
#
# https://github.com/jketterl/owrx_connector
#
# In order to use Perseus HF you need to install the libperseus-sdr
#
# https://github.com/Microtelecom/libperseus-sdr
#
# and do the proper changes to the sdrs object below
# (see also Wiki in https://github.com/jketterl/openwebrx/wiki/Sample-configuration-for-Perseus-HF-receiver).
#

sdrs = {
    "rtlsdr": {
        "name": "RTL-SDR",
        "type": "rtl_sdr_soapy",
        "ppm": 0,
        # you can change this if you use an upconverter. formula is:
        # center_freq + lfo_offset = actual frequency on the sdr
        # "lfo_offset": 0,
        "profiles": {
            "wx": {
                "name": "Weather Band",
                "center_freq": 162350000,
                "rf_gain": 20,
                "samp_rate": 1000000,
                "start_freq":  162400000,
                "start_mod": "nfm",
            },
            "2m": {
                "name": "2m Band",
                "center_freq": 145000000,
                "rf_gain": 20,
                "samp_rate": 2400000,
                "start_freq": 145725000,
                "start_mod": "nfm",
            },
            "70cm": {
                "name": "70cm Band",
                "center_freq": 435500000,
                "rf_gain": 20,
                "samp_rate": 2400000,
                "start_freq": 4355000000,
                "start_mod": "nfm",
            },
            "fm": {
                "name": "FM Band",
                "center_freq": 91000000,
                "rf_gain": 20,
                "samp_rate": 2400000,
                "start_freq": 90000000,
                "start_mod": "nfm",
            },
            "10mv": {
                "name": "10m Band",
                "center_freq": 28000000,
                "rf_gain": 20,
                "samp_rate": 2400000,
                "start_freq": 28500000,
                "start_mod": "usb",
            },
            "6m": {
                "name": "6m Band",
                "center_freq": 51000000,
                "rf_gain": 20,
                "samp_rate": 2400000,
                "start_freq": 51000000,
                "start_mod": "usb",
            },
            "110MHz": {
                "name": "110 MHz",
                "center_freq": 110000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 110000000,
                "start_mod": "nfm",
            },
            "112MHz": {
                "name": "112 MHz",
                "center_freq": 112000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 112000000,
                "start_mod": "nfm",
            },
            "114MHz": {
                "name": "114 MHz",
                "center_freq": 114000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 114000000,
                "start_mod": "nfm",
            },
            "116mhz": {
                "name": "116 MHz",
                "center_freq": 116000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 116000000,
                "start_mod": "nfm",
            },
            "118mhz": {
                "name": "118 MHz Air Band",
                "center_freq": 118000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 118000000,
                "start_mod": "nfm",
            },
            "120mhz": {
                "name": "120 MHz Air Band",
                "center_freq": 120000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 120000000,
                "start_mod": "nfm",
            },
            "122mhz": {
                "name": "122 MHz Air Band",
                "center_freq": 122000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 122000000,
                "start_mod": "nfm",
            },
            "124mhz": {
                "name": "124 MHz Air Band",
                "center_freq": 124000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 124000000,
                "start_mod": "nfm",
            },
            "126mhz": {
                "name": "126 MHz Air Band",
                "center_freq": 126000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 126000000,
                "start_mod": "nfm",
            },
            "128mhz": {
                "name": "126 MHz Air Band",
                "center_freq": 128000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 128000000,
                "start_mod": "nfm",
            },
            "130mhz": {
                "name": "130 MHz Air Band",
                "center_freq": 130000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 130000000,
                "start_mod": "nfm",
            },
            "132mhz": {
                "name": "132 MHz Air Band",
                "center_freq": 132000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 132000000,
                "start_mod": "nfm",
            },
            "134mhz": {
                "name": "134 MHz Air Band",
                "center_freq": 134000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 134000000,
                "start_mod": "nfm",
            },
            "136mhz": {
                "name": "136 MHz Air Band",
                "center_freq": 136000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 136000000,
                "start_mod": "nfm",
            },
            "138mhz": {
                "name": "138 MHz NOAA Weather Satellite Band",
                "center_freq": 138000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 138000000,
                "start_mod": "nfm",
            },
            "140mhz": {
                "name": "140 MHz",
                "center_freq": 140000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 140000000,
                "start_mod": "nfm",
            },
            "142mhz": {
                "name": "142 MHz",
                "center_freq": 142000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 142000000,
                "start_mod": "nfm",
            },
            "144mhz": {
                "name": "144 MHz Ham Band",
                "center_freq": 144000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 144000000,
                "start_mod": "nfm",
            },
            "146mhz": {
                "name": "146 MHz Ham Band",
                "center_freq": 146000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 146000000,
                "start_mod": "nfm",
            },
            "148mhz": {
                "name": "148 MHz Ham Band",
                "center_freq": 148000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 148000000,
                "start_mod": "nfm",
            },
            "150mhz": {
                "name": "150 MHz",
                "center_freq": 150000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 150000000,
                "start_mod": "nfm",
            },
            "152mhz": {
                "name": "152 MHz",
                "center_freq": 152000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 152000000,
                "start_mod": "nfm",
            },
            "154mhz": {
                "name": "154 MHz",
                "center_freq": 154000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 154000000,
                "start_mod": "nfm",
            },
            "156mhz": {
                "name": "156 MHz",
                "center_freq": 156000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 156100000,
                "start_mod": "nfm",
            },
            "158mhz": {
                "name": "158 MHz",
                "center_freq": 158000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 158000000,
                "start_mod": "nfm",
            },
            "160mhz": {
                "name": "160 MHz",
                "center_freq": 160000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 160000000,
                "start_mod": "nfm",
            },
            "162mhz": {
                "name": "162 MHz Weather Band",
                "center_freq": 162000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 162000000,
                "start_mod": "nfm",
            },
            "164mhz": {
                "name": "164 MHz",
                "center_freq": 164000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 164000000,
                "start_mod": "nfm",
            },
            "166mhz": {
                "name": "166 MHz",
                "center_freq": 166000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 166000000,
                "start_mod": "nfm",
            },
            "168mhz": {
                "name": "168 MHz",
                "center_freq": 168000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 168000000,
                "start_mod": "nfm",
            },
            "170mhz": {
                "name": "170 MHz",
                "center_freq": 170000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 170000000,
                "start_mod": "nfm",
            },
            "172mhz": {
                "name": "172 MHz",
                "center_freq": 172000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 172000000,
                "start_mod": "nfm",
            },
            "174mhz": {
                "name": "174 MHz",
                "center_freq": 174000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 174000000,
                "start_mod": "nfm",
            },
           "420mhz": {
                "name": "420 MHz 70cm Ham Band",
                "center_freq": 420000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 420000000,
                "start_mod": "nfm",
            },
            "422mhz": {
                "name": "422 MHz 70cm Ham Band",
                "center_freq": 422000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 422000000,
                "start_mod": "nfm",
            },
            "424mhz": {
                "name": "424 MHz 70cm Ham Band",
                "center_freq": 424000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 424000000,
                "start_mod": "nfm",
            },
            "426mhz": {
                "name": "426 MHz 70cm Ham Band",
                "center_freq": 426000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 426000000,
                "start_mod": "nfm",
            },
            "428mhz": {
                "name": "426 MHz 70cm Ham Band",
                "center_freq": 428000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 428000000,
                "start_mod": "nfm",
            },
            "430mhz": {
                "name": "430 MHz 70cm Ham Band",
                "center_freq": 430000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 430000000,
                "start_mod": "nfm",
            },
            "432mhz": {
                "name": "432 MHz 70cm Ham Band",
                "center_freq": 432000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 432000000,
                "start_mod": "nfm",
            },
            "434mhz": {
                "name": "434 MHz 70cm Ham Band",
                "center_freq": 434000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 434000000,
                "start_mod": "nfm",
            },
            "436mhz": {
                "name": "436 MHz 70cm Ham Band",
                "center_freq": 436000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 436000000,
                "start_mod": "nfm",
            },
            "438mhz": {
                "name": "438 MHz 70cm Ham Band",
                "center_freq": 138000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 438000000,
                "start_mod": "nfm",
            },
            "446mhz": {
                "name": "446 MHz",
                "center_freq": 446000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 446000000,
                "start_mod": "nfm",
            },
            "460mhz": {
                "name": "460 MHz",
                "center_freq": 460000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 460000000,
                "start_mod": "nfm",
            },
            "462mhz": {
                "name": "462 MHz FRS/GMRS",
                "center_freq": 462000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 462000000,
                "start_mod": "nfm",
            },
            "467mhz": {
                "name": "467 MHz FRS/GMRS",
                "center_freq": 467000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 467000000,
                "start_mod": "nfm",
            },
            "860mhz": {
                "name": "860 MHz",
                "center_freq": 860000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 860000000,
                "start_mod": "nfm",
            },
            "862mhz": {
                "name": "862 MHz",
                "center_freq": 862000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 862000000,
                "start_mod": "nfm",
            },
            "864mhz": {
                "name": "864 MHz",
                "center_freq": 864000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 864000000,
                "start_mod": "nfm",
            },
            "866mhz": {
                "name": "866 MHz",
                "center_freq": 866000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 866000000,
                "start_mod": "nfm",
            },
            "868mhz": {
                "name": "868 MHz SRD Band",
                "center_freq": 868000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 868000000,
                "start_mod": "nfm",
            },
            "870mhz": {
                "name": "870 MHz",
                "center_freq": 870000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 870000000,
                "start_mod": "nfm",
            },
            "915mhz": {
                "name": "915 MHz ISM Band",
                "center_freq": 915000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 915000000,
                "start_mod": "nfm",
            },
            "1090mhz": {
                "name": "1090 MHz",
                "center_freq": 1090000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1090000000,
                "start_mod": "nfm",
            },
            "1880mhz": {
                "name": "1880 MHz",
                "center_freq": 1880000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1880000000,
                "start_mod": "nfm",
            },
            "1882mhz": {
                "name": "1882 MHz",
                "center_freq": 1882000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1882000000,
                "start_mod": "nfm",
            },
            "1884mhz": {
                "name": "1884 MHz",
                "center_freq": 1884000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1884000000,
                "start_mod": "nfm",
            },
            "1886mhz": {
                "name": "1886 MHz",
                "center_freq": 1886000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1886000000,
                "start_mod": "nfm",
            },
            "1888mhz": {
                "name": "1888 MHz",
                "center_freq": 1888000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1888000000,
                "start_mod": "nfm",
            },
            "1890mhz": {
                "name": "1890 MHz",
                "center_freq": 1890000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1890000000,
                "start_mod": "nfm",
            },
            "1892mhz": {
                "name": "1892 MHz",
                "center_freq": 1892000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1892000000,
                "start_mod": "nfm",
            },
            "1894mhz": {
                "name": "1894 MHz",
                "center_freq": 1894000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1894000000,
                "start_mod": "nfm",
            },
            "1896mhz": {
                "name": "1896 MHz",
                "center_freq": 1896000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1896000000,
                "start_mod": "nfm",
            },
            "1898mhz": {
                "name": "1898 MHz",
                "center_freq": 1898000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1898000000,
                "start_mod": "nfm",
            },
            "1900mhz": {
                "name": "1900 MHz",
                "center_freq": 1900000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1900000000,
                "start_mod": "nfm",
            },
            "23cm": {
                "name": "23cm Ham Band",
                "center_freq": 1270000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 1270000000,
                "start_mod": "nfm",
            },
            "13cm": {
                "name": "13cm Ham Band",
                "center_freq": 2370000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 2370000000,
                "start_mod": "nfm",
            },
            "9cm": {
                "name": "9cm Band",
                "center_freq": 3440000000,
                "rf_gain": 30,
                "samp_rate": 2400000,
                "start_freq": 3440000000,
                "start_mod": "nfm",
            },
            "10m": {
                "name": "10m Ham Band HF",
                "center_freq": 29150000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 28283000,
                "start_mod": "usb",
                "direct_sampling": 2,
            },
            "19m": {
                "name": "19m Broadcast Band HF",
                "center_freq": 16000000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 15400000,
                "start_mod": "am",
                "direct_sampling": 2,
            },
            "20m": {
                "name": "20m Ham Band HF",
                "center_freq": 14150000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 14070000,
                "start_mod": "usb",
                "direct_sampling": 2,
            },
            "30m": {
                "name": "30m Ham Band HF",
                "center_freq": 10125000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 10142000,
                "start_mod": "usb",
                "direct_sampling": 2,
            },
            "40m": {
                "name": "40m Ham Band HF",
                "center_freq": 7100000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 7070000,
                "start_mod": "lsb",
                "direct_sampling": 2,
            },
            "49m": {
                "name": "49m Broadcast Band HF",
                "center_freq": 6000000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 6070000,
                "start_mod": "am",
                "direct_sampling": 2,
            },
            "80m": {
                "name": "80m Ham Band HF",
                "center_freq": 3650000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 3570000,
                "start_mod": "lsb",
                "direct_sampling": 2,
            },
            "160m": {
                "name": "160m Band HF",
                "center_freq": 1700000,
                "rf_gain": 10,
                "samp_rate": 2400000,
                "start_freq": 1440000,
                "start_mod": "am",
                "direct_sampling": 2,
            },
        },
    },    
}

# ==== Color themes ====

# A guide is available to help you set these values: https://github.com/simonyiszk/openwebrx/wiki/Calibrating-waterfall-display-levels

### default theme by teejez:
waterfall_colors = [0x000000FF, 0x0000FFFF, 0x00FFFFFF, 0x00FF00FF, 0xFFFF00FF, 0xFF0000FF, 0xFF00FFFF, 0xFFFFFFFF]
waterfall_min_level = -88  # in dB
waterfall_max_level = -20
waterfall_auto_level_margin = {"min": 5, "max": 40}
### old theme by HA7ILM:
# waterfall_colors = "[0x000000ff,0x2e6893ff, 0x69a5d0ff, 0x214b69ff, 0x9dc4e0ff,  0xfff775ff, 0xff8a8aff, 0xb20000ff]"
# waterfall_min_level = -115 #in dB
# waterfall_max_level = 0
# waterfall_auto_level_margin = {"min": 20, "max": 30}
##For the old colors, you might also want to set [fft_voverlap_factor] to 0.

# Note: When the auto waterfall level button is clicked, the following happens:
#   [waterfall_min_level] = [current_min_power_level] - [waterfall_auto_level_margin["min"]]
#   [waterfall_max_level] = [current_max_power_level] + [waterfall_auto_level_margin["max"]]
#
#   ___|________________________________________|____________________________________|________________________________________|___> signal power
#        \_waterfall_auto_level_margin["min"]_/ |__ current_min_power_level          | \_waterfall_auto_level_margin["max"]_/
#                                                          current_max_power_level __|

# === Experimental settings ===
# Warning! The settings below are very experimental.
csdr_dynamic_bufsize = False  # This allows you to change the buffering mode of csdr.
csdr_print_bufsizes = False  # This prints the buffer sizes used for csdr processes.
csdr_through = False  # Setting this True will print out how much data is going into the DSP chains.

nmux_memory = 50  # in megabytes. This sets the approximate size of the circular buffer used by nmux.

google_maps_api_key = ""

# how long should positions be visible on the map?
# they will start fading out after half of that
# in seconds; default: 2 hours
map_position_retention_time = 2 * 60 * 60

# decoder queue configuration
# due to the nature of some operating modes (ft8, ft8, jt9, jt65, wspr and js8), the data is recorded for a given amount
# of time (6 seconds up to 2 minutes) and decoded at the end. this can lead to very high peak loads.
# to mitigate this, the recordings will be queued and processed in sequence.
# the number of workers will limit the total amount of work (one worker will losely occupy one cpu / thread)
decoding_queue_workers = 2
# the maximum queue length will cause decodes to be dumped if the workers cannot keep up
# if you are running background services, make sure this number is high enough to accept the task influx during peaks
# i.e. this should be higher than the number of decoding services running at the same time
decoding_queue_length = 10

# wsjt decoding depth will allow more results, but will also consume more cpu
wsjt_decoding_depth = 3
# can also be set for each mode separately
# jt65 seems to be somewhat prone to erroneous decodes, this setting handles that to some extent
wsjt_decoding_depths = {"jt65": 1}

# JS8 comes in different speeds: normal, slow, fast, turbo. This setting controls which ones are enabled.
js8_enabled_profiles = ["normal", "slow"]
# JS8 decoding depth; higher value will get more results, but will also consume more cpu
js8_decoding_depth = 3

temporary_directory = "/tmp"

services_enabled = False
services_decoders = ["ft8", "ft4", "wspr", "packet"]

# === aprs igate settings ===
# if you want to share your APRS decodes with the aprs network, configure these settings accordingly
aprs_callsign = "N0CALL"
aprs_igate_enabled = False
aprs_igate_server = "euro.aprs2.net"
aprs_igate_password = ""
# beacon uses the receiver_gps setting, so if you enable this, make sure the location is correct there
aprs_igate_beacon = False

# path to the aprs symbols repository (get it here: https://github.com/hessu/aprs-symbols)
aprs_symbols_path = "/opt/aprs-symbols/png"

# === PSK Reporter setting ===
# enable this if you want to upload all ft8, ft4 etc spots to pskreporter.info
# this also uses the receiver_gps setting from above, so make sure it contains a correct locator
pskreporter_enabled = False
pskreporter_callsign = "N0CALL"

# === Web admin settings ===
# this feature is experimental at the moment. it should not be enabled on shared receivers since it allows remote
# changes to the receiver settings. enable for testing in controlled environment only.
# webadmin_enabled = False
