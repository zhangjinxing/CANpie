//============================================================================//
// File:          qcan_pcan_basic.cpp                                         //
// Description:   PCAN Basic library handling                                 //
//                                                                            //
// Copyright (C) MicroControl GmbH & Co. KG                                   //
// 53842 Troisdorf - Germany                                                  //
// www.microcontrol.net                                                       //
//                                                                            //
//----------------------------------------------------------------------------//
// Redistribution and use in source and binary forms, with or without         //
// modification, are permitted provided that the following conditions         //
// are met:                                                                   //
// 1. Redistributions of source code must retain the above copyright          //
//    notice, this list of conditions, the following disclaimer and           //
//    the referenced file 'COPYING'.                                          //
// 2. Redistributions in binary form must reproduce the above copyright       //
//    notice, this list of conditions and the following disclaimer in the     //
//    documentation and/or other materials provided with the distribution.    //
// 3. Neither the name of MicroControl nor the names of its contributors      //
//    may be used to endorse or promote products derived from this software   //
//    without specific prior written permission.                              //
//                                                                            //
// Provided that this notice is retained in full, this software may be        //
// distributed under the terms of the GNU Lesser General Public License       //
// ("LGPL") version 3 as distributed in the 'COPYING' file.                   //
//                                                                            //
//============================================================================//

#include "qcan_pcan_basic.hpp"

//----------------------------------------------------------------------------//
// QCanPcanBasic()                                                            //
//                                                                            //
//----------------------------------------------------------------------------//
QCanPcanBasic::QCanPcanBasic()
{
   // get file name
   clCanLibP.setFileName(QCAN_PEAKLIB);

   if (!clCanLibP.load()) {
       qCritical() << "QCanPeakUsb(): Failed to load the library:" << qPrintable(clCanLibP.fileName());
   }

   //----------------------------------------------------------------
   // Loads API functions
   //
   pfnCAN_InitializeP = (QCanPcanBasic::CAN_Initialize_tf) clCanLibP.resolve("CAN_Initialize");
   #if QCAN_SUPPORT_CAN_FD > 0
   pfnCAN_InitializeFDP = (QCanPcanBasic::CAN_InitializeFD_tf) clCanLibP.resolve("CAN_InitializeFD");
   #endif
   pfnCAN_UninitializeP = (QCanPcanBasic::CAN_Uninitialize_tf)clCanLibP.resolve("CAN_Uninitialize");
   pfnCAN_ResetP = (QCanPcanBasic::CAN_Reset_tf)clCanLibP.resolve("CAN_Reset");
   pfnCAN_GetStatusP = (QCanPcanBasic::CAN_GetStatus_tf)clCanLibP.resolve("CAN_GetStatus");
   pfnCAN_ReadP = (QCanPcanBasic::CAN_Read_tf)clCanLibP.resolve("CAN_Read");
   #if QCAN_SUPPORT_CAN_FD > 0
   pfnCAN_ReadFDP = (QCanPcanBasic::CAN_ReadFD_tf)clCanLibP.resolve("CAN_ReadFD");
   #endif
   pfnCAN_WriteP = (QCanPcanBasic::CAN_Write_tf)clCanLibP.resolve("CAN_Write");
   #if QCAN_SUPPORT_CAN_FD > 0
   pfnCAN_WriteFDP = (QCanPcanBasic::CAN_WriteFD_tf)clCanLibP.resolve("CAN_WriteFD");
   #endif
   pfnCAN_FilterMessagesP = (QCanPcanBasic::CAN_FilterMessages_tf)clCanLibP.resolve("CAN_FilterMessages");
   pfnCAN_GetValueP = (QCanPcanBasic::CAN_GetValue_tf)clCanLibP.resolve("CAN_GetValue");
   pfnCAN_SetValueP = (QCanPcanBasic::CAN_SetValue_tf)clCanLibP.resolve("CAN_SetValue");
   pfnCAN_GetErrorTextP = (QCanPcanBasic::CAN_GetErrorText_tf)clCanLibP.resolve("CAN_GetErrorText");


   //----------------------------------------------------------------
   // check for success
   //
   btLibFuncLoadP =  pfnCAN_InitializeP &&
                     #if QCAN_SUPPORT_CAN_FD > 0
                     pfnCAN_InitializeFDP &&
                     #endif
                     pfnCAN_UninitializeP &&
                     pfnCAN_ResetP &&
                     pfnCAN_GetStatusP &&
                     pfnCAN_ReadP  &&
                     pfnCAN_WriteP &&
                     #if QCAN_SUPPORT_CAN_FD > 0
                     pfnCAN_FilterMessagesP &&
                     pfnCAN_ReadFDP &&
                     pfnCAN_WriteFDP &&
                     #endif
                     pfnCAN_GetValueP &&
                     pfnCAN_SetValueP &&
                     pfnCAN_GetErrorTextP;

   //----------------------------------------------------------------
   // If the API was not loaded (Wrong version), an error message is shown.
   //
   if (!btLibFuncLoadP)
   {
      qCritical() << "QCanPcanBasic::QCanPcanBasic() CRITICAL: Fail to load some library functions!";
   }

   qInfo() << "QCanPcanBasic::QCanPcanBasic() INFO: All library functions succesfully loaded!";
}

//----------------------------------------------------------------------------//
// ~QCanPcanBasic()                                                           //
//                                                                            //
//----------------------------------------------------------------------------//
QCanPcanBasic::~QCanPcanBasic()
{
   qDebug() << "QCanPcanBasic::~QCanPcanBasic()";

   btLibFuncLoadP = false;

   //------------------------------------------------------------------
   // Unload library and remove all API functions
   //
   if (clCanLibP.isLoaded())
   {
      clCanLibP.unload();
   }

   pfnCAN_InitializeP = NULL;
   pfnCAN_UninitializeP = NULL;
   pfnCAN_ResetP = NULL;
   pfnCAN_GetStatusP = NULL;
   pfnCAN_ReadP = NULL;
   pfnCAN_WriteP = NULL;
   pfnCAN_FilterMessagesP = NULL;
   pfnCAN_GetValueP = NULL;
   pfnCAN_SetValueP = NULL;
   pfnCAN_GetErrorTextP = NULL;

   #if QCAN_SUPPORT_CAN_FD > 0
   pfnCAN_InitializeFDP = NULL;
   pfnCAN_ReadFDP = NULL;
   pfnCAN_WriteFDP = NULL;
   #endif
}

//----------------------------------------------------------------------------//
// isAvailable()                                                              //
//                                                                            //
//----------------------------------------------------------------------------//
bool QCanPcanBasic::isAvailable()
{
   return btLibFuncLoadP;

}

//----------------------------------------------------------------------------//
// formatedError()                                                            //
//                                                                            //
//----------------------------------------------------------------------------//
QString QCanPcanBasic::formatedError(TPCANStatus tvErrorV)
{
   TPCANStatus tvStatusT;
   char aszBufferT[256];
   QString clResultT;

   memset(aszBufferT,'\0',256);

   // Gets the text using the GetErrorText API function
   // If the function success, the translated error is returned. If it fails,
   // a text describing the current error is returned.
   //

   tvStatusT = pfnCAN_GetErrorTextP(tvErrorV,0x00,aszBufferT);
   if(tvStatusT != PCAN_ERROR_OK)
   {
      clResultT = ("An error ocurred. Error-code's text ("+ QString::number(tvErrorV,16).toUpper() + "h) couldn't be retrieved");
   } else
   {
      clResultT = QString::number(tvErrorV,16).toUpper() + "h : " + QLatin1String(aszBufferT);
   }

   return clResultT;
}


TPCANStatus QCanPcanBasic::initialize     (TPCANHandle uwChannelV, TPCANBaudrate uwBtr0Btr1V, TPCANType ubHwTypeV, DWORD ulIOPortV, WORD uwInterruptV)
{
   return pfnCAN_InitializeP(uwChannelV, uwBtr0Btr1V, ubHwTypeV, ulIOPortV, uwInterruptV);
}


#if QCAN_SUPPORT_CAN_FD > 0
TPCANStatus QCanPcanBasic::initializeFD(TPCANHandle uwChannelV,
                                        TPCANBitrateFD pszBitrateFDV)
{
   return pfnCAN_InitializeFDP(uwChannelV, pszBitrateFDV);
}
#endif


TPCANStatus QCanPcanBasic::unInitialize   (TPCANHandle uwChannelV)
{
   return pfnCAN_UninitializeP(uwChannelV);
}

TPCANStatus QCanPcanBasic::reset          (TPCANHandle uwChannelV)
{
   return pfnCAN_ResetP(uwChannelV);
}

TPCANStatus QCanPcanBasic::getStatus      (TPCANHandle uwChannelV)
{
   return pfnCAN_GetStatusP(uwChannelV);
}

TPCANStatus QCanPcanBasic::read           (TPCANHandle uwChannelV, TPCANMsg *ptsMessageBufferV, TPCANTimestamp *tsTimestampBufferV)
{
   return pfnCAN_ReadP(uwChannelV,ptsMessageBufferV,tsTimestampBufferV);
}

#if QCAN_SUPPORT_CAN_FD > 0
TPCANStatus QCanPcanBasic::readFD(TPCANHandle uwChannelV,
                                  TPCANMsgFD *ptsMessageBufferV,
                                  TPCANTimestampFD *puqTimestampBufferV)
{
   return pfnCAN_ReadFDP(uwChannelV,ptsMessageBufferV,puqTimestampBufferV);
}
#endif


TPCANStatus QCanPcanBasic::write          (TPCANHandle uwChannelV, TPCANMsg *ptsMessageBufferV)
{
   return pfnCAN_WriteP(uwChannelV,ptsMessageBufferV);
}

#if QCAN_SUPPORT_CAN_FD > 0
TPCANStatus QCanPcanBasic::writeFD( TPCANHandle uwChannelV,
                                    TPCANMsgFD *ptsMessageBufferV)
{
   return pfnCAN_WriteFDP(uwChannelV,ptsMessageBufferV);
}
#endif

TPCANStatus QCanPcanBasic::filterMessages (TPCANHandle uwChannelV, DWORD ulFromIDV, DWORD ulToIDV, TPCANMode ubModeV)
{
   return pfnCAN_FilterMessagesP(uwChannelV,ulFromIDV,ulToIDV,ubModeV);
}

TPCANStatus QCanPcanBasic::getValue       (TPCANHandle uwChannelV, TPCANParameter ubParameterV, void *pvdBufferV, DWORD ulBufferLengthV)
{
   return pfnCAN_GetValueP(uwChannelV,ubParameterV,pvdBufferV,ulBufferLengthV);
}

TPCANStatus QCanPcanBasic::setValue       (TPCANHandle uwChannelV, TPCANParameter ubParameterV, void *pvdBufferV, DWORD ulBufferLengthV)
{
   return pfnCAN_SetValueP(uwChannelV,ubParameterV,pvdBufferV,ulBufferLengthV);
}

TPCANStatus QCanPcanBasic::getErrorText   (TPCANStatus ulErrorV, WORD uwLanguageV, LPSTR pszBufferV)
{
   return pfnCAN_GetErrorTextP(ulErrorV, uwLanguageV, pszBufferV);
}
