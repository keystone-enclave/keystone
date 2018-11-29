//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "report.h"
#include <iostream>
#include <cstring> 
int main(void)
{
  byte bytes[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

  Report report;
  Report copy;

  struct report_t rpt;

  rpt.enclave.data_len = 5;

  report.fromBytes((byte*)&rpt);
  copy.fromJson( report.stringfy() );

  report.printJson();
  copy.printJson();

  std::cout << report.verify() << " " << copy.verify() << std::endl;
}
