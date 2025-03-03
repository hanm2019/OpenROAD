///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <string.h>

#include <array>
#include <fstream>
#include <string>

#include "ZException.h"
#include "dbObject.h"
#include "map"
#include "odb.h"
#include "tuple"

namespace odb {

class _dbDatabase;

class dbOStream
{
  _dbDatabase* _db;
  FILE* _f;
  double _lef_area_factor;
  double _lef_dist_factor;

  void write_error()
  {
    throw ZException("write failed on database stream; system io error: (%s)",
                     strerror(ferror(_f)));
  }

 public:
  dbOStream(_dbDatabase* db, FILE* f);

  _dbDatabase* getDatabase() { return _db; }

  dbOStream& operator<<(bool c)
  {
    unsigned char b = (c == true ? 1 : 0);
    return *this << b;
  }

  dbOStream& operator<<(char c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(unsigned char c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(short c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(unsigned short c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(int c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(uint64_t c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(unsigned int c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(int8_t c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(float c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(double c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(long double c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  dbOStream& operator<<(const char* c)
  {
    if (c == NULL) {
      *this << 0;
    } else {
      int l = strlen(c) + 1;
      *this << l;
      int n = fwrite(c, l, 1, _f);
      if (n != 1)
        write_error();
    }

    return *this;
  }

  dbOStream& operator<<(dbObjectType c)
  {
    int n = fwrite(&c, sizeof(c), 1, _f);
    if (n != 1)
      write_error();
    return *this;
  }

  template <class T1, class T2>
  dbOStream& operator<<(const std::pair<T1, T2>& p)
  {
    *this << p.first;
    *this << p.second;
    return *this;
  }

  template <size_t I = 0, typename... Ts>
  constexpr dbOStream& operator<<(const std::tuple<Ts...>& tup)
  {
    if constexpr (I == sizeof...(Ts)) {
      return *this;
    } else {
      *this << std::get<I>(tup);
      return ((*this).operator<< <I + 1>(tup));
    }
  }

  template <class T1, class T2>
  dbOStream& operator<<(const std::map<T1, T2>& m)
  {
    uint sz = m.size();
    *this << sz;
    for (auto const& [key, val] : m) {
      *this << key;
      *this << val;
    }
    return *this;
  }

  template <class T, std::size_t SIZE>
  dbOStream& operator<<(const std::array<T, SIZE>& a)
  {
    for (auto& val : a) {
      *this << val;
    }
    return *this;
  }

  dbOStream& operator<<(std::string s)
  {
    char* tmp = strdup(s.c_str());
    *this << tmp;
    free((void*) tmp);
    return *this;
  }

  double lefarea(int value) { return ((double) value * _lef_area_factor); }

  double lefdist(int value) { return ((double) value * _lef_dist_factor); }
};

class dbIStream
{
  std::ifstream& _f;
  _dbDatabase* _db;
  double _lef_area_factor;
  double _lef_dist_factor;

 public:
  dbIStream(_dbDatabase* db, std::ifstream& f);

  _dbDatabase* getDatabase() { return _db; }

  dbIStream& operator>>(bool& c)
  {
    unsigned char b;
    *this >> b;
    c = (b == 1 ? true : false);
    return *this;
  }

  dbIStream& operator>>(char& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(unsigned char& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(short& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(unsigned short& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(int& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(uint64_t& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(unsigned int& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(int8_t& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(float& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(double& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(long double& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  dbIStream& operator>>(char*& c)
  {
    int l;
    *this >> l;

    if (l == 0)
      c = NULL;
    else {
      c = (char*) malloc(l);
      _f.read(reinterpret_cast<char*>(c), l);
    }

    return *this;
  }

  dbIStream& operator>>(dbObjectType& c)
  {
    _f.read(reinterpret_cast<char*>(&c), sizeof(c));
    return *this;
  }

  template <class T1, class T2>
  dbIStream& operator>>(std::pair<T1, T2>& p)
  {
    *this >> p.first;
    *this >> p.second;
    return *this;
  }
  template <class T1, class T2>
  dbIStream& operator>>(std::map<T1, T2>& m)
  {
    uint sz;
    *this >> sz;
    for (uint i = 0; i < sz; i++) {
      T1 key;
      T2 val;
      *this >> key;
      *this >> val;
      m[key] = val;
    }
    return *this;
  }
  template <class T, std::size_t SIZE>
  dbIStream& operator>>(std::array<T, SIZE>& a)
  {
    for (std::size_t i = 0; i < SIZE; i++) {
      *this >> a[i];
    }
    return *this;
  }

  template <size_t I = 0, typename... Ts>
  constexpr dbIStream& operator>>(std::tuple<Ts...>& tup)
  {
    if constexpr (I == sizeof...(Ts)) {
      return *this;
    } else {
      *this >> std::get<I>(tup);
      return ((*this).operator>><I + 1>(tup));
    }
  }

  dbIStream& operator>>(std::string& s)
  {
    char* tmp;
    *this >> tmp;
    s = std::string(tmp);
    free((void*) tmp);
    return *this;
  }

  double lefarea(int value) { return ((double) value * _lef_area_factor); }

  double lefdist(int value) { return ((double) value * _lef_dist_factor); }
};

}  // namespace odb
