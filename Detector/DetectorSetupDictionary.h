/********************************************************************
* /Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error /Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableDetectorSetupDictionary();
extern void G__cpp_setup_inheritanceDetectorSetupDictionary();
extern void G__cpp_setup_typetableDetectorSetupDictionary();
extern void G__cpp_setup_memvarDetectorSetupDictionary();
extern void G__cpp_setup_globalDetectorSetupDictionary();
extern void G__cpp_setup_memfuncDetectorSetupDictionary();
extern void G__cpp_setup_funcDetectorSetupDictionary();
extern void G__set_cpp_environmentDetectorSetupDictionary();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "src/Atmosphere.h"
#include "src/Array.h"
#include "src/Station.h"
#include "src/Channel.h"
#include "src/ChannelGain.h"
#include "src/ChannelMap.h"
#include "src/Detector.h"
#include "src/Antenna.h"
#include "src/DetectorSetup.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__DetectorSetupDictionaryLN_TClass;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TBuffer;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMemberInspector;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TObject;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_string;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_pairlEdoublecOdoublegR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TElementActionTlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TElementPosActionTlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTRow_constlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTRowlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTDiag_constlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTColumn_constlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTFlat_constlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTSub_constlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTSparseRow_constlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTSparseDiag_constlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTColumnlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTDiaglEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTFlatlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTSublEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTSparseRowlEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TMatrixTSparseDiaglEfloatgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TRotation;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TVector3;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEdoublecOutlcLcLLinearInterpolatorlEdoublegRmUcOlesslEdoublegRcOallocatorlEpairlEconstsPdoublecOutlcLcLLinearInterpolatorlEdoublegRmUgRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_Station;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_Array;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEintcOStationcOlesslEintgRcOallocatorlEpairlEconstsPintcOStationgRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEintcOStationcOlesslEintgRcOallocatorlEpairlEconstsPintcOStationgRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEintcOTVector3cOlesslEintgRcOallocatorlEpairlEconstsPintcOTVector3gRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_TH2D;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_ChannelGain;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_Antenna;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_Channel;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEpairlEunsignedsPintcOunsignedsPintgRcOeSignalSourceTypecOlesslEpairlEunsignedsPintcOunsignedsPintgRsPgRcOallocatorlEpairlEconstsPpairlEunsignedsPintcOunsignedsPintgRcOeSignalSourceTypegRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEpairlEunsignedsPintcOunsignedsPintgRcOunsignedsPintcOlesslEpairlEunsignedsPintcOunsignedsPintgRsPgRcOallocatorlEpairlEconstsPpairlEunsignedsPintcOunsignedsPintgRcOunsignedsPintgRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEpairlEunsignedsPintcOunsignedsPintgRcOpairlEunsignedsPintcOunsignedsPintgRcOlesslEpairlEunsignedsPintcOunsignedsPintgRsPgRcOallocatorlEpairlEconstsPpairlEunsignedsPintcOunsignedsPintgRcOpairlEunsignedsPintcOunsignedsPintgRsPgRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEunsignedsPintcOpairlEdoublecOdoublegRcOlesslEunsignedsPintgRcOallocatorlEpairlEconstsPunsignedsPintcOpairlEdoublecOdoublegRsPgRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_maplEunsignedsPintcOdoublecOlesslEunsignedsPintgRcOallocatorlEpairlEconstsPunsignedsPintcOdoublegRsPgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_DetectorSetup;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_vectorlEChannelcOallocatorlEChannelgRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_reverse_iteratorlEvectorlEChannelcOallocatorlEChannelgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_vectorlEAntennacOallocatorlEAntennagRsPgR;
extern G__linked_taginfo G__DetectorSetupDictionaryLN_reverse_iteratorlEvectorlEAntennacOallocatorlEAntennagRsPgRcLcLiteratorgR;

/* STUB derived class for protected member access */