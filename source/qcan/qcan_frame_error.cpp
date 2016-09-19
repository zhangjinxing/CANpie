//============================================================================//
// File:          qcan_frame.cpp                                              //
// Description:   QCAN Server - Network implementation                        //
// Author:        Uwe Koppe                                                   //
// e-mail:        koppe@microcontrol.net                                      //
//                                                                            //
// Copyright (C) MicroControl GmbH & Co. KG                                   //
// Junkersring 23                                                             //
// 53844 Troisdorf                                                            //
// Germany                                                                    //
// Tel: +49-2241-25659-0                                                      //
// Fax: +49-2241-25659-11                                                     //
//                                                                            //
// The copyright to the computer program(s) herein is the property of         //
// MicroControl GmbH & Co. KG, Germany. The program(s) may be used            //
// and/or copied only with the written permission of MicroControl GmbH &      //
// Co. KG or in accordance with the terms and conditions stipulated in        //
// the agreement/contract under which the program(s) have been supplied.      //
//----------------------------------------------------------------------------//
//                                                                            //
// Date        History                                                        //
// ----------  -------------------------------------------------------------- //
// 20.05.2015  Initial version                                                //
//                                                                            //
//============================================================================//


//------------------------------------------------------------------------------
// SVN  $Date: 2014-09-15 18:50:11 +0200 (Mo, 15. Sep 2014) $
// SVN  $Rev: 6156 $ --- $Author: koppe $
//------------------------------------------------------------------------------


/*----------------------------------------------------------------------------*\
** Include files                                                              **
**                                                                            **
\*----------------------------------------------------------------------------*/

#include <QDebug>
#include "qcan_frame_error.hpp"

/*----------------------------------------------------------------------------*\
** Definitions                                                                **
**                                                                            **
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
** Class methods                                                              **
**                                                                            **
\*----------------------------------------------------------------------------*/


//----------------------------------------------------------------------------//
// QCanFrameError()                                                           //
// constructor                                                                //
//----------------------------------------------------------------------------//
QCanFrameError::QCanFrameError()
{

   setFrameType(eTYPE_QCAN_ERR);
   setDlc(4);
}


//----------------------------------------------------------------------------//
// QCanFrame()                                                                //
// destructor                                                                 //
//----------------------------------------------------------------------------//
QCanFrameError::~QCanFrameError()
{
   
}


//----------------------------------------------------------------------------//
// errorCounterReceive()                                                      //
// use data byte 2 for storage                                                //
//----------------------------------------------------------------------------//
uint8_t QCanFrameError::errorCounterReceive(void) const
{
   return(data(2));
}


//----------------------------------------------------------------------------//
// errorCounterTransmit()                                                     //
// use data byte 3 for storage                                                //
//----------------------------------------------------------------------------//
uint8_t QCanFrameError::errorCounterTransmit(void) const
{
   return(data(3));
}


//----------------------------------------------------------------------------//
// errorState()                                                               //
// use data byte 0 for storage                                                //
//----------------------------------------------------------------------------//
QCan::CAN_State_e QCanFrameError::errorState(void) const
{
   return((QCan::CAN_State_e) data(0));
}


//----------------------------------------------------------------------------//
// errorType()                                                                //
// use data byte 1 for storage                                                //
//----------------------------------------------------------------------------//
QCanFrameError::ErrorType_e QCanFrameError::errorType(void) const
{
   return((QCanFrameError::ErrorType_e) data(1));
}


//----------------------------------------------------------------------------//
// setErrorCounterReceive()                                                   //
// use data byte 2 for storage                                                //
//----------------------------------------------------------------------------//
void QCanFrameError::setErrorCounterReceive(const uint8_t & ubErrorCntR)
{
   setData(2, ubErrorCntR);
}


//----------------------------------------------------------------------------//
// setErrorCounterTransmit()                                                  //
// use data byte 3 for storage                                                //
//----------------------------------------------------------------------------//
void QCanFrameError::setErrorCounterTransmit(const uint8_t & ubErrorCntR)
{
   setData(3, ubErrorCntR);
}


//----------------------------------------------------------------------------//
// setErrorStat()                                                             //
// use data byte 0 for storage                                                //
//----------------------------------------------------------------------------//
void QCanFrameError::setErrorState(const QCan::CAN_State_e & ubStateR)
{
   setData(0, ubStateR);
}


//----------------------------------------------------------------------------//
// setErrorType()                                                             //
// use data byte 1 for storage                                                //
//----------------------------------------------------------------------------//
void QCanFrameError::setErrorType(const ErrorType_e & ubTypeR)
{
   setData(1, ubTypeR);
}

//----------------------------------------------------------------------------//
// toString()                                                                 //
// print CAN frame                                                            //
//----------------------------------------------------------------------------//
QString QCanFrameError::toString(const bool & btShowTimeR)
{
   //----------------------------------------------------------------
   // setup a string object
   //
   QString clStringT;
   
   if(btShowTimeR == true)
   {
      
   }
   

   //----------------------------------------------------------------
   // print frame format
   //
   clStringT += "CAN error frame   ";

   switch(this->errorState())
   {
      case QCan::eCAN_STATE_BUS_ACTIVE:
         clStringT += "Error active";
         break;

      case QCan::eCAN_STATE_BUS_WARN:
         clStringT += "Warning level reached";
         break;

      case QCan::eCAN_STATE_BUS_PASSIVE:
         clStringT += "Error passive";
         break;

      case QCan::eCAN_STATE_BUS_OFF:
         clStringT += "Bus off";
         break;

      default:

         break;
   }
   
   return(clStringT);
}
      