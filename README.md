# MicroSIP

MicroSIP - open source portable SIP softphone based on PJSIP stack for Windows OS.

## Description

MicroSIP is a lightweight SIP softphone for Windows that allows high quality VoIP calls via open SIP protocol. It supports:

- Voice calls (person-to-person or on regular telephones)
- Video calls (H.264, H.263+, VP8)
- SIMPLE messaging (RFC 3428) and presence (RFC 3903, 6665)
- DTMF (In-band, RFC2833, SIP-INFO)
- Multiple voice codecs: Opus, G.711 A-law and μ-law, G.722, G.721.1, G.723, G.729, GSM, AMR, AMR-WB, iLBC, Speex, SILK
- WebRTC echo cancellation and voice activity detection
- TLS/SRTP encryption for control and media
- Multilanguage support

## Features

- Small footprint (>2.5MB) and low RAM usage (>5MB)
- Written in C and C++
- No additional dependencies
- Stores settings in ini file
- Portable version available

## Building

This project requires:
- Visual Studio 2015 or later
- Windows SDK
- PJSIP library and dependencies

### GitHub Actions Build

This repository includes GitHub Actions workflows for automatic building of MicroSIP.exe installer packages.

## License

GNU GPL v2 - see [LICENSE](LICENSE) file for details.

## Original Project

This is a mirror of the MicroSIP project from https://www.microsip.org/

Original author: MicroSIP Team
