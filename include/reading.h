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

#ifndef _READING_H_
#define _READING_H_

#include <sys/time.h>
#include <string.h>

#include "obis.h"
#include <shared_ptr.hpp>
#include <meter_protocol.hpp>

#define MAX_IDENTIFIER_LEN 255

/* Identifiers */
class ReadingIdentifier {
public:
	typedef vz::shared_ptr<ReadingIdentifier> Ptr;

	virtual size_t unparse(char *buffer, size_t n) = 0;
  int operator==( ReadingIdentifier &cmp);
    int compare( ReadingIdentifier *lhs,  ReadingIdentifier *rhs);

  protected:
  explicit ReadingIdentifier() {};

  private:
  ReadingIdentifier (const ReadingIdentifier& original);
  ReadingIdentifier& operator= (const ReadingIdentifier& rhs);
};

class ObisIdentifier : public ReadingIdentifier {

  public:
	typedef vz::shared_ptr<ObisIdentifier> Ptr;

  ObisIdentifier(Obis obis) : _obis(obis) {}
    size_t unparse(char *buffer, size_t n);
    int operator==(ObisIdentifier &cmp);
    
    const Obis &obis() const { return _obis; }
    
  private:
  ObisIdentifier (const ObisIdentifier& original);
  ObisIdentifier& operator= (const ObisIdentifier& rhs);

  protected:
	Obis _obis;
};

class StringIdentifier : public ReadingIdentifier {
public:
	StringIdentifier(std::string s) : _string(s) {}
    size_t unparse(char *buffer, size_t n);
    bool operator==(StringIdentifier &cmp);
protected:
	std::string _string;
};

class UuidIdentifier : public ReadingIdentifier {
public:
  size_t unparse(char *buffer, size_t n);
	bool operator==(UuidIdentifier &cmp);
protected:
	std::string _uuid;
};

class ChannelIdentifier : public ReadingIdentifier {
public:
	ChannelIdentifier(int channel) : _channel(channel) {}
    size_t unparse(char *buffer, size_t n);
    bool operator==(ChannelIdentifier &cmp);
protected:
	int _channel;
};


class Reading {

public:
	Reading(ReadingIdentifier::Ptr pIndentifier);
	Reading(double pValue, struct timeval pTime, ReadingIdentifier::Ptr pIndentifier);

  // copy-operator!
  double tvtod(struct timeval tv);
  struct timeval dtotv(double ts);

  const double value() const { return _value; }
  
  /**
   * Print identifier to buffer for debugging/dump
   *
   * @return the amount of bytes used in buffer
   */
  size_t unparse(meter_protocol_t protocol, char *buffer, size_t n);

  protected:
	double _value;
	struct timeval time;
	ReadingIdentifier::Ptr identifier;
};

/**
 * Parse identifier by a given string and protocol
 *
 * @param protocol the given protocol context in which the string should be parsed
 * @param string the string-encoded identifier
 * @return 0 on success, < 0 on error
 */
int reading_id_parse(meter_protocol_t protocol, ReadingIdentifier *id, const char *string);


#endif /* _READING_H_ */