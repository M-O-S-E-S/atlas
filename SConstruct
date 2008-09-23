
# Base paths for external libraries (platform dependent)
if str(Platform()) == 'win32':
   # HLA RTI
   rtiPath = 'L:/rtis-1.3_D18A'
   # libxml2
   xmlPath = 'L:/libxml2-2.6.27'
   # iconv
   iconvPath = 'L:/iconv-1.9.1'
   # inttypes.h for MSVC
   msinttypesPath = 'L:/msinttypes-r21'
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
   
# Embeds a Visual Studio-style manifest into the given output target
# (only under Windows)
def embedManifest(environment, target, suffix):
   # The suffix indicates the file type (1=.exe, 2=.dll)
   environment.AddPostAction(target,
                             'mt.exe -nologo -manifest ${TARGET}.manifest \
                             -outputresource:$TARGET;' + str(suffix))

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
         atSem.c++ atShm.c++ atSpawn.c++ atTime.c++'

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
   # /Z7          = Generate debug information
   compileFlags = Split('/nologo /MD /O2 /EHsc /W3 /Zc:forScope /GR /Gd /Z7')

   # Additional flags to disable useless warnings in Windows
   compileFlags += Split('/wd4091 /wd4275 /wd4290')

   # Disable deprecation warnings for "insecure" and "nonstandard" functions
   # in Windows
   defines += Split('_CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_DEPRECATE')

   # Flags for the VC++ linker
   # /DEBUG          = Generate debugging information
   # /OPT:REF        = Optimize away unreferenced code
   # /OPT:ICF        = Optimize away redundant function packages
   # /INCREMENTAL:NO = Do not perform incremental linking
   linkFlags = Split('/DEBUG /OPT:REF /OPT:ICF /INCREMENTAL:NO')
else:
   # Flags for gcc (generate debug information and optimize)
   compileFlags = Split('-g -O')

   # No linker flags
   linkFlags = []


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

   # Add msinttypes headers
   extIncPath.extend(Split(msinttypesPath + '/include'))

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
basisEnv.Append(CCFLAGS = compileFlags)
basisEnv.Append(CPPDEFINES = defines)
basisEnv.Append(CPPPATH = incPath)
basisEnv.Append(LIBPATH = libPath)
basisEnv.Append(LIBS = libs)
basisEnv.Append(LINKFLAGS = linkFlags)


# Now, compile the shared library for ATLAS
atlasLib = basisEnv.SharedLibrary('atlas', source = atlasSource)


# Under Windows, embed the manifest into the .dll
if str(Platform()) == 'win32':
   embedManifest(basisEnv, atlasLib, 2)

