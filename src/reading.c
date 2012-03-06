/**
 * Reading related functions
 *
 * @package vzlogger
 * @copyright Copyright (c) 2011, The volkszaehler.org project
 * @license http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author Steffen Vogel <info@steffenvogel.de>
 */
/*
 * This file is part of volkzaehler.org
 *
 * volkzaehler.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * volkzaehler.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with volkszaehler.org. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <math.h>

#include "reading.h"
//#include "meter_protocol.h"

Reading::Reading(ReadingIdentifier::Ptr pIndentifier)
    : _value(0)
                //    , time(0)
    , identifier(pIndentifier)
{
  
}
Reading::Reading(
  double pValue
  , struct timeval pTime
  , ReadingIdentifier::Ptr pIndentifier
  )
    : _value(pValue)
    , time(pTime)
    , identifier(pIndentifier)
{
}

double Reading::tvtod(struct timeval tv) {
	return tv.tv_sec + tv.tv_usec / 1e6;
}

struct timeval Reading::dtotv(double ts) {
	double integral;
	double fraction = modf(ts, &integral);

	struct timeval tv;
  tv.tv_usec = (long int) (fraction * 1e6);
  tv.tv_sec = (long int) integral;

	return tv;
}

ReadingIdentifier::Ptr reading_id_parse(meter_protocol_t protocol, const char *string) {
//int reading_id_parse(meter_protocol_t protocol, ReadingIdentifier::Ptr, const char *string) {
  ReadingIdentifier::Ptr rid;
  
	switch (protocol) {
		case meter_protocol_d0:
		case meter_protocol_sml:
      rid = ReadingIdentifier::Ptr(new ObisIdentifier(Obis((unsigned char*)string)));
/*			if (obis_parse(string, &id->obis) != SUCCESS) {
				if (obis_lookup_alias(string, &id->obis) != SUCCESS) {
					throw std::exception("");
          //return ERR;
				}
			}
*/
			break;

		case meter_protocol_fluksov2: {
			char type[13];
			int channel;

			int ret = sscanf(string, "sensor%u/%12s", &channel, type);
			if (ret != 2) {
        throw std::exception();
        //return ERR;
			}
      rid = ReadingIdentifier::Ptr(new ChannelIdentifier(channel+1));

			//id->channel = channel + 1; /* increment by 1 to distinguish between +0 and -0 */

#if 0
			if (strcmp(type, "consumption") == 0) {
				id->channel *= -1;
			}
			else if (strcmp(type, "power") != 0) {
        throw std::exception("");
				//return ERR;
			}
#endif
			break;
		}

		case meter_protocol_file:
		case meter_protocol_exec:
      rid = ReadingIdentifier::Ptr(new StringIdentifier(string));
			//id->string = strdup(string); // TODO free() elsewhere
			break;

		default: /* ignore other protocols which do not provide id's */
			break;
	}

  return rid;
	//return SUCCESS;
}

// reading_id_unparse
size_t Reading::unparse(
  meter_protocol_t protocol,
  char *buffer, size_t n
  ) {
  return identifier->unparse(buffer, n);
  
#if 0
	switch (protocol) {
		case meter_protocol_d0:
		case meter_protocol_sml:
			//obis_unparse(id.obis, buffer, n);
			break;

		case meter_protocol_fluksov2:
			//snprintf(buffer, n, "sensor%u/%s", abs(id.channel) - 1, (id.channel > 0) ? "power" : "consumption");
			break;

		case meter_protocol_file:
		case meter_protocol_exec:
			//if (id.string != NULL) {
      //		strncpy(buffer, id.string, n);
      //		break;
			//}

		default:
			buffer[0] = '\0';
	}

	return strlen(buffer);
#endif
}

int ReadingIdentifier::operator==( ReadingIdentifier &cmp) {
  return this->compare(this, &cmp);
}

int ReadingIdentifier::compare( ReadingIdentifier *lhs,  ReadingIdentifier *rhs) {
  if(ObisIdentifier* lhsx = dynamic_cast<ObisIdentifier*>(lhs)) {
    if(ObisIdentifier* rhsx = dynamic_cast<ObisIdentifier*>(rhs)) {
      return lhsx == rhsx;
    } else { return -1; }
  } else 
    if( StringIdentifier* lhsx = dynamic_cast<StringIdentifier*>(rhs)) {
      if(StringIdentifier* rhsx = dynamic_cast<StringIdentifier*>(lhs)) {
        return lhsx == rhsx;
    } else { return -1; }
  } else 
  if(UuidIdentifier* lhsx = dynamic_cast<UuidIdentifier*>(lhs)) {
    if(UuidIdentifier* rhsx = dynamic_cast<UuidIdentifier*>(rhs)) {
        return lhsx == rhsx;
    } else { return -1; }
  } else 
  if(ChannelIdentifier* lhsx = dynamic_cast<ChannelIdentifier*>(lhs)) {
    if(ChannelIdentifier* rhsx = dynamic_cast<ChannelIdentifier*>(rhs)) {
        return lhsx == rhsx;
    } else { return -1; }
  }
  return -1;
}

size_t ObisIdentifier::unparse(char *buffer, size_t n) {
  return _obis.unparse(buffer, n);
}
int ObisIdentifier::operator==(ObisIdentifier &cmp) {
  return (_obis == cmp.obis());
}

size_t StringIdentifier::unparse(char *buffer, size_t n) {
  if (_string != "") {
    strncpy(buffer, _string.c_str(), n);
  }
	return strlen(buffer);
}

size_t UuidIdentifier::unparse(char *buffer, size_t n) {
  if (_uuid != "") {
    strncpy(buffer, _uuid.c_str(), n);
  }
	return strlen(buffer);
}

size_t ChannelIdentifier::unparse(char *buffer, size_t n) {
  snprintf(buffer, n, "sensor%u/%s", abs(_channel) - 1, (_channel > 0) ? "power" : "consumption");
	return strlen(buffer);
}