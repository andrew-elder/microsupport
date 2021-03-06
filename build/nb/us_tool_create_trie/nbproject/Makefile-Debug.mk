#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1534608076/us_tool_create_trie.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../microsupport/dist/Debug/GNU-MacOSX/libmicrosupport.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-MacOSX/us_tool_create_trie

dist/Debug/GNU-MacOSX/us_tool_create_trie: ../microsupport/dist/Debug/GNU-MacOSX/libmicrosupport.a

dist/Debug/GNU-MacOSX/us_tool_create_trie: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-MacOSX
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/us_tool_create_trie ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1534608076/us_tool_create_trie.o: ../../../tools-dev/us_tool_create_trie.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1534608076
	${RM} $@.d
	$(COMPILE.c) -g -I../../../include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1534608076/us_tool_create_trie.o ../../../tools-dev/us_tool_create_trie.c

# Subprojects
.build-subprojects:
	cd ../microsupport && ${MAKE}  -f Makefile CONF=Debug
	cd ../microsupport && ${MAKE}  -f Makefile CONF=Debug
	cd ../microsupport && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-MacOSX/us_tool_create_trie

# Subprojects
.clean-subprojects:
	cd ../microsupport && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../microsupport && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../microsupport && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
