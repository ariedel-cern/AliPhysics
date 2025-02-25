# **************************************************************************
# * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************/

#Module
set(MODULE PWGLFrsnextra)
add_definitions(-D_MODULE_="${MODULE}")

# Module include folder
include_directories(${AliPhysics_SOURCE_DIR}/PWGLF/RESONANCES/extra)

# Additional includes - alphabetical order except ROOT
include_directories(${ROOT_INCLUDE_DIRS}
                    ${AliPhysics_SOURCE_DIR}/CORRFW
		    ${AliPhysics_SOURCE_DIR}/OADB
                    ${AliPhysics_SOURCE_DIR}/PWG/Tools
                    ${AliPhysics_SOURCE_DIR}/PWG/muon
                    ${AliPhysics_SOURCE_DIR}/PWG/DevNanoAOD 
                    ${AliPhysics_SOURCE_DIR}/PWGLF/RESONANCES
                    ${AliPhysics_SOURCE_DIR}/OADB/COMMON/MULTIPLICITY		    
                    ${AliPhysics_SOURCE_DIR}/PWGPP/EVCHAR/FlowVectorCorrections/QnCorrections
                    ${AliPhysics_SOURCE_DIR}/PWGPP/EVCHAR/FlowVectorCorrections/QnCorrectionsInterface
		    ${AliPhysics_SOURCE_DIR}/PWG/FLOW/Base		    
                   )

# Sources - alphabetical order
set(SRCS
  AliRsnTOFT0maker.cxx 
  AliRsnTOFCorrectionESD.cxx 
  AliRsnAnalysisPhi900GeV.cxx 
  AliRsnAnalysisPhi7TeV.cxx 
  AliAnalysisTaskSigma1385.cxx
  AliXiStar.cxx
  AliXiStarpp13TeV.cxx
  AliXiStarPbPb.cxx
  AliXiStarPbPb_RUN2.cxx
  AliXiStarEventCollection.cxx
  AliXiStarpp13TeVEventCollection.cxx
  AliXiStarPbPbEventCollection.cxx
  AliAnalysisTaskLambdaStar.cxx
  AliAnalysisTaskSEF01710fromAODtracks.cxx
  AliRsnf0f2Task.cxx
  AliAnalysisTaskPPVsMultMCRSN.cxx
  AliAnalysisTaskEffMatrix.cxx
  RESOSA/AliAnalysisTaskPhiSA.cxx
  RESOSA/AliAnalysisTaskPhiSAR.cxx
  AliAnalysisTaskPhiCount.cxx
  AliAnalysisTask_Phi_MC.cxx
  AliAnalysisTaskAODTrackPairUtils.cxx
  AliAnalysisTaskAODTrackPair.cxx
  AliAnalysisTaskAODTrackPairMC.cxx
  )

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}")

# Add a shared library
add_library_tested(${MODULE} SHARED  ${SRCS} G__${MODULE}.cxx)

# Generate the ROOT map
# Dependecies
set(LIBDEPS ANALYSISalice CDB CORRFW OADB PWGPPevcharQnInterface TOFbase TOFrec PWGTools)
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Linking the library
target_link_libraries(${MODULE} ${LIBDEPS})

# Public include folders that will be propagated to the dependecies
target_include_directories(${MODULE} PUBLIC ${incdirs})

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Installation
install(TARGETS ${MODULE} 
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
install(FILES ${HDRS} DESTINATION include)
install(FILES AddTaskXiStar.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskLambda.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskRsnf0f2.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskF01710fromAODtracks.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskXiStarpp13TeV.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskXiStarPbPb.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskXiStarPbPb_RUN2.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskPPVsMultMCRSN.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddAnalysisTaskEffMatrix.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddAnalysisTaskPhiCount.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddAnalysisTask_Phi_MC.C DESTINATION PWGLF/RESONANCES/extra)
install(FILES AddTaskAODTrackPair.C DESTINATION PWGLF/RESONANCES/extra)

# Install directories
install(DIRECTORY RESOSA DESTINATION PWGLF/RESONANCES/extra)


