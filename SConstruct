
# Base paths for external libraries (platform dependent)
if str(Platform()) == 'win32':
   # HLA RTI
   rtiPath = 'L:/rtis-1.3_D18A'
   # libxml2
   xmlPath = 'L:/libxml2-2.6.27'
   # iconv
   iconvPath = 'L:/iconv-1.9.1'
else:
   # HLA RTI
   rtiPath = '/irl/tools/libs/rtis-1.3_D18A'
   # libxml2 (see subpaths below)
   xmlPath = '/usr'


# Borrowed from id, this takes a prefix and adds it to each filename and
# then returns them as a list
def buildList(sPrefix, sString):
   sList = Split(sString)
   for i in range(len(sList)):
      sList[i] = sPrefix + '/' + sList[i]
   return sList

# This takes a base path, a subpath include path, a subpath lib path
# and a string of libs, and then adds the appropriate data to our
# internal variables (include path, lib path and list of libs)
def addExternal(basePath, subIncPath, subLibPath, libs):
   includeDir = Split(basePath + subIncPath)
   libDir = Split(basePath + subLibPath)
   extIncPath.extend(includeDir)
   extLibPath.extend(libDir)
   extLibs.extend(Split(libs))
   

# Create an exportable environment; this will be used as the basis
# environment for all of the modules we create (though they can add
# additional element to it if necessary)
basisEnv = Environment()


# Build-up subdirs and sublists of files within ATLAS
communicationDir = 'communication'
communicationSrc = 'atIPCInterface.c++ \
                    atNetworkInterface.c++ \
                    atUDPNetworkInterface.c++ atTCPNetworkInterface.c++ \
                    atSharedMemoryInterface.c++ atSharedQueue.c++ \
                    atThreadInterface.c++ atThreadQueue.c++ \
                    atSerialInterface.c++ \
                    atHLAInterface.c++ \
                    atRTIInterface.c++ atRTIInterfaceAmbassador.c++ \
                    atNameValuePair.c++ atKeyedBufferHandler.c++'

containerDir = 'container'
containerSrc = 'atArray.c++ atList.c++ atMap.c++ atPriorityQueue.c++'

foundationDir = 'foundation'
foundationSrc = 'atNotifier.c++ atItem.c++'

mathDir = 'math'
mathSrc = 'atVector.c++ atMatrix.c++ atQuat.c++'

osDir = 'os'
osSrc = 'atByteSwap.c++ atDynamic.c++ atErrno.c++ atNetwork.c++ \
         atSem.c++ atShm.c++ atTime.c++'

utilDir = 'util'
utilSrc = 'atConfigFile.c++ atBufferHandler.c++ \
           atString.c++ atStringBuffer.c++ atStringTokenizer.c++ \
           atTimer.c++ atJoint.c++'

xmlDir = 'xml'
xmlSrc = 'atXMLBuffer.c++ atXMLDocument.c++ atXMLReader.c++'

# Collect together all the source files that make up ATLAS
atlasSource = buildList(communicationDir, communicationSrc)
atlasSource.extend(buildList(containerDir, containerSrc))
atlasSource.extend(buildList(foundationDir, foundationSrc))
atlasSource.extend(buildList(mathDir, mathSrc))
atlasSource.extend(buildList(osDir, osSrc))
atlasSource.extend(buildList(utilDir, utilSrc))
atlasSource.extend(buildList(xmlDir, xmlSrc))


# Set the initial defines and flags

# Set-up ATLAS symbols for exporting
defines = Split('ATLAS_SYM=EXPORT')

# Then handle platform-specific issues
if str(Platform()) == 'win32':
   # Flags for the VC++ compiler
   # /nologo      = Don't print the compiler banner
   # /MD          = Use multithreaded DLL runtime
   # /O2          = Optimize for speed
   # /EHsc        = Exception handling
   # /W3          = Warning level
   # /Zc:forScope = Use standard C++ scoping rules in for loops
   # /GR          = Enable C++ run-time type information
   # /Gd          = Use __cdecl calling convention
   # /Zi          = Generate debug information
   flags = Split('/nologo /MD /O2 /EHsc /W3 /Zc:forScope /GR /Gd /Zi')

   # Additional flags to disable useless warnings in Windows
   flags += Split('/wd4091 /wd4275 /wd4290')

   # Set-up ATLAS symbols for exporting
   defines = Split('ATLAS_SYM=EXPORT')

   # Disable deprecation warnings for "insecure" and "nonstandard" functions
   # in Windows
   defines += Split('_CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_DEPRECATE')
else:
   # Flags for gcc (generate debug information and optimize)
   flags = Split('-g -O')


# Set the initial paths and libraries
incPath = Split('container foundation math os util')
libPath = Split('')
libs = Split('')


# Initialize our external used library information (include path, lib path
# and libs)
extIncPath = []
extLibPath = []
extLibs = []

# Depending on platform, add the external libraries that ATLAS requires
# (Windows requires more to be linked in than Linux does)
if str(Platform()) == 'win32':
   # Add the RTI
   addExternal(rtiPath, '/include/1.3', '/lib/winnt_vc++-8.0', 'librti13')

   # Add libxml2
   addExternal(xmlPath, '/include', '/lib', 'libxml2')

   # Add iconv
   addExternal(iconvPath, '/include', '/lib', 'iconv')

   # Add the Windows-specific libraries (already in main path)
   extLibs.extend(Split('ws2_32 winmm'))
else:
   # Add the RTI
   addExternal(rtiPath, '/include/1.3', '/lib/linux_g++-4.1', 'rti13')

   # Add libxml2
   addExternal(xmlPath, '/include/libxml2', '/lib', 'xml2')

# Finally, add the paths for all the external used libraries
# to the main paths (and libraries)
incPath.extend(extIncPath)
libPath.extend(extLibPath)
libs.extend(extLibs)


# Add the elements to the environment
basisEnv.Append(CCFLAGS = flags)
basisEnv.Append(CPPDEFINES = defines)
basisEnv.Append(CPPPATH = incPath)
basisEnv.Append(LIBPATH = libPath)
basisEnv.Append(LIBS = libs)


# Now, compile the shared library for ATLAS
basisEnv.SharedLibrary('atlas', source = atlasSource)

