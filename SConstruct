
# Import sys so we call exit() if we need to
import sys


# Figure out whether we're on a 32-bit or 64-bit system
import struct
arch = ARGUMENTS.get('arch', str(struct.calcsize("P") * 8) + 'bit')

# Get the build target (e.g., win32, posix, etc.)
buildTarget = ARGUMENTS.get('platform', str(Platform()))

# If the buildTarget isn't for mobile (not "ios" or "android") then add
# bit info (to support 32-bit or 64-bit builds)
if buildTarget != 'ios' and buildTarget != 'android':
   buildTarget = buildTarget + '.' + arch

# Echo for user
print 'Building for ' + buildTarget + '...'


# Settings for optional components to include
enableBluetooth = ARGUMENTS.get('enableBluetooth', 'yes').lower();
enableRTI = ARGUMENTS.get('enableRTI', 'no').lower();
enableUUID = ARGUMENTS.get('enableUUID', 'yes').lower();
enableXML = ARGUMENTS.get('enableXML', 'yes').lower();


# Base paths for external libraries (platform dependent)
if buildTarget == 'win32.64bit':
   # libxml2
   xmlPath = ARGUMENTS.get('xmlPath', 'L:/libxml2-2.9.2')
   # iconv
   iconvPath = ARGUMENTS.get('iconvPath', 'L:/libiconv-1.14')
   # inttypes.h for MSVC
   msinttypesPath = ARGUMENTS.get('msinttypesPath', 'L:/msinttypes-r26')
   # Bluetooth (disabled for Windows)
   bluetoothPath = ARGUMENTS.get('bluetoothPath', '')
   # HLA RTI
   rtiPath = ARGUMENTS.get('rtiPath', 'L:/rtis-1.3_D22')
elif buildTarget == 'win32.32bit':
   # libxml2
   xmlPath = ARGUMENTS.get('xmlPath', 'L:/libxml2-2.7.6')
   # iconv
   iconvPath = ARGUMENTS.get('iconvPath', 'L:/iconv-1.9.1')
   # inttypes.h for MSVC
   msinttypesPath = ARGUMENTS.get('msinttypesPath', 'L:/msinttypes-r26')
   # Bluetooth (disabled for Windows)
   bluetoothPath = ARGUMENTS.get('bluetoothPath', '')
   # HLA RTI
   rtiPath = ARGUMENTS.get('rtiPath', 'L:/rtis-1.3_D22')
elif buildTarget == 'posix.64bit':
   # uuid
   uuidPath = ARGUMENTS.get('uuidPath', '/usr')
   # libxml2 (see subpaths below)
   xmlPath = ARGUMENTS.get('xmlPath', '/usr')
   # bluetooth (see subpaths below)
   bluetoothPath = ARGUMENTS.get('bluetoothPath', '/usr')
   # HLA RTI
   rtiPath = ARGUMENTS.get('rtiPath', '/irl/tools/libs/rtis_D30G')
elif buildTarget == 'posix.32bit':
   # uuid
   uuidPath = ARGUMENTS.get('uuidPath', '/irl/tools/libs/uuid-1.6.2')
   # libxml2 (see subpaths below)
   xmlPath = ARGUMENTS.get('xmlPath', '/usr')
   # bluetooth (see subpaths below)
   bluetoothPath = ARGUMENTS.get('bluetoothPath', '/usr')
   # HLA RTI
   rtiPath = ARGUMENTS.get('rtiPath', '/irl/tools/libs/rtis-1.3_D22')
elif buildTarget == 'ios':
   # Xcode
   xcodePath = ARGUMENTS.get('xcodePath', '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer');
   # uuid
   uuidPath = ARGUMENTS.get('uuidPath', '/irl/tools-ios/libs/uuid-1.6.2')
   # libxml2
   xmlPath = ARGUMENTS.get('xmlPath', xcodePath + '/SDKs/iPhoneOS8.2.sdk/usr');
   # bluetooth (empty means do not include)
   bluetoothPath = ARGUMENTS.get('bluetoothPath', '')
   # HLA RTI
   rtiPath = ARGUMENTS.get('rtiPath', '')
elif buildTarget == 'android':
   # Android NDK
   ndkPath = ARGUMENTS.get('ndkPath', '/irl/tools/libs/android-ndk-r8')
   # uuid
   uuidPath = ARGUMENTS.get('uuidPath', '/irl/tools-android/libs/uuid-1.6.2')
   # libxml2
   xmlPath = ARGUMENTS.get('xmlPath', '/irl/tools-android/libs/libxml2-2.7.8')
   # bluetooth (empty means do not include)
   bluetoothPath = ARGUMENTS.get('bluetoothPath', '')
   # HLA RTI
   rtiPath = ARGUMENTS.get('rtiPath', '')
   # glob
   globPath = ARGUMENTS.get('globPath', '/irl/tools-android/libs/glob')
else:
   # Unsupported architecture so bail
   print "Unsupported target type ", buildTarget
   sys.exit(0)


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
# NOTE: For 32-bit Windows, we have to force the target architecture or
#       Scons will default to 64-bit; we also limit to VS2010 for now
if buildTarget == 'win32.32bit':
   basisEnv = Environment(MSVC_VERSION='10.0', TARGET_ARCH='x86')
else:
   basisEnv = Environment()

# Build-up subdirs and sublists of files within ATLAS
communicationDir = 'communication'
communicationSrc = 'atIPCInterface.c++ \
                    atNetworkInterface.c++ \
                    atUDPNetworkInterface.c++ atTCPNetworkInterface.c++ \
                    atSharedMemoryInterface.c++ atSharedQueue.c++ \
                    atThreadInterface.c++ atThreadQueue.c++ atThreadCount.c++ \
                    atSerialInterface.c++ \
                    atNameValuePair.c++ atKeyedBufferHandler.c++ \
                    atHLAInterface.c++ atRTIInterface.c++'
if enableBluetooth == 'yes':
   communicationSrc = communicationSrc + \
                      ' atBluetoothInterface.c++ atRFCOMMBluetoothInterface.c++'
if enableRTI == 'yes':
   communicationSrc = communicationSrc + ' atRTIInterfaceAmbassador.c++'


containerDir = 'container'
containerSrc = 'atPair.c++ atTriple.c++ atArray.c++ atList.c++ atMap.c++ \
                atPriorityQueue.c++'

foundationDir = 'foundation'
foundationSrc = 'atNotifier.c++ atItem.c++'

mathDir = 'math'
mathSrc = 'atVector.c++ atMatrix.c++ atQuat.c++'

osDir = 'os'
osSrc = 'atByteSwap.c++ atDynamic.c++ atErrno.c++ atFile.c++ \
         atNetwork.c++ atSem.c++ atShm.c++ atSpawn.c++ atThread.c++ \
         atTime.c++'
if enableBluetooth == 'yes':
   osSrc = osSrc + ' atBluetooth.c++'
if enableUUID == 'yes':
   osSrc = osSrc + ' atUUID.c++'

utilDir = 'util'
utilSrc = 'atConfigFile.c++ atBufferHandler.c++ atPath.c++ atImage.c++ \
           atString.c++ atStringBuffer.c++ atStringTokenizer.c++ \
           atTimer.c++ atJoint.c++ atCommandLine.c++ \
           atChar.c++ atInt.c++ atLong.c++ atUInt64.c++ atFloat.c++ \
           atDouble.c++'

xmlDir = 'xml'
xmlSrc = 'atXMLBuffer.c++ atXMLDocument.c++ atXMLReader.c++'

# Collect together all the source files that make up ATLAS
atlasSource = buildList(communicationDir, communicationSrc)
atlasSource.extend(buildList(containerDir, containerSrc))
atlasSource.extend(buildList(foundationDir, foundationSrc))
atlasSource.extend(buildList(mathDir, mathSrc))
atlasSource.extend(buildList(osDir, osSrc))
atlasSource.extend(buildList(utilDir, utilSrc))
if enableXML == 'yes':
   atlasSource.extend(buildList(xmlDir, xmlSrc))


# Set the initial defines and flags

# Set-up ATLAS symbols for exporting
defines = Split('ATLAS_SYM=EXPORT')

# Set-up common settings (between all platforms)
# Add a constant define to mark that we included the RTI
if enableBluetooth == 'yes':
   defines += Split('__AT_BLUETOOTH_ENABLED__')
if enableRTI == 'yes':
   defines += Split('__AT_RTI_ENABLED__')
if enableUUID == 'yes':
   defines += Split('__AT_UUID_ENABLED__')
if enableXML == 'yes':
   defines += Split('__AT_XML_ENABLED__')

# Then handle platform-specific issues
if buildTarget == 'win32.64bit':
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
   compileFlags = Split('/nologo /MD /O2 /EHsc /W3 /Zc:forScope /GR /Gd /Zi')

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
   # /MANIFEST       = Generate manifest file
   linkFlags = Split('/DEBUG /OPT:REF /OPT:ICF /INCREMENTAL:NO /MANIFEST')
elif buildTarget == 'win32.32bit':
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
   compileFlags = Split('/nologo /MD /O2 /EHsc /W3 /Zc:forScope /GR /Gd /Zi')

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
   # /MANIFEST       = Generate manifest file
   linkFlags = Split('/DEBUG /OPT:REF /OPT:ICF /INCREMENTAL:NO /MANIFEST')
   linkFlags += Split('/MACHINE:X86')
elif buildTarget == 'posix.64bit':
   # Flags for gcc (generate debug information and optimize,
   # use POSIX and not GNU)
   compileFlags = Split('-g -O -Wno-write-strings -fpermissive')
   compileFlags += Split('-D_POSIX_C_SOURCE=200112L')

   # No linker flags
   linkFlags = []
elif buildTarget == 'posix.32bit':
   # Flags for gcc (generate debug information and optimize,
   # use POSIX and not GNU)
   compileFlags = Split('-g -O -Wno-write-strings -D_POSIX_C_SOURCE=200112L')

   # No linker flags
   linkFlags = []
elif buildTarget == 'ios':
   # Set-up cross compiler by platform we're actually on
   if str(Platform()) == 'darwin':
      # Specify the cross compiler
      basisEnv.Replace(CC = '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain' +
                            '/usr/bin/' +
                            'clang')
      basisEnv.Replace(CXX = '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain' +
                             '/usr/bin/' +
                             'clang')

      # No compile flags
      compileFlags = Split('-Wno-parentheses -Wno-deprecated-writable-strings' +
                           ' -arch armv7 -isysroot ' + xcodePath + 
                           '/SDKs/iPhoneOS8.2.sdk')

      # Set a define so things can know we're cross compiling for iOS
      defines += Split('__IOS__ _DARWIN_C_SOURCE')

      # No linker flags
      linkFlags = Split(' -arch armv7 -isysroot ' + xcodePath + 
                        '/SDKs/iPhoneOS8.2.sdk -lobjc -lstdc++')
   else:
      print "Unsupported platform type for cross compile", buildTarget
      sys.exit(0)
elif buildTarget == 'android':
   # Set-up cross compiler by platform we're actually on
   if str(Platform()) == 'posix':
      # Specify the ARM EABI cross compiler
      basisEnv.Replace(CC = ndkPath + 
                            '/toolchains/arm-linux-androideabi-4.4.3' +
                            '/prebuilt/linux-x86/bin/' +
                            'arm-linux-androideabi-gcc')
      basisEnv.Replace(CXX = ndkPath + 
                             '/toolchains/arm-linux-androideabi-4.4.3' +
                             '/prebuilt/linux-x86/bin/' +
                             'arm-linux-androideabi-g++')

      # Flags for the ARM EABI compiler
      # -mandroid          = Target the android platform
      # -fexceptions       = Explicitly include exceptions (TODO: remove?)
      # -Wno-write-strings = Don't throw warnings for string const conversions
      compileFlags = Split('-mandroid -fexceptions -Wno-write-strings -frtti')

      # Set a define so things can know we're cross compiling for Android
      defines += Split('__ANDROID__')

      # Specify the system root for the Android NDK's linker
      linkFlags = Split('--sysroot=' + ndkPath + 
                        '/platforms/android-14/arch-arm')
   else:
      print "Unsupported platform type for cross compile", buildTarget
      sys.exit(0)
else:
   # Unsupported architecture so bail
   print "Unsupported target type ", buildTarget
   sys.exit(0)


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
if buildTarget == 'win32.64bit':
   # Add the RTI
   if enableRTI == 'yes':
      addExternal(rtiPath, '/include/1.3', '/lib/winnt_vc++-10.0',
                  'librti13 libfedtime13 rtis mcast snpr parser')

   # Add libxml2 (and its dependency, iconv)
   if enableXML == 'yes':
      addExternal(xmlPath, '/include/libxml2', '/lib', 'libxml2')
      addExternal(iconvPath, '/include', '/lib', 'iconv')

   # Add msinttypes headers
   extIncPath.extend(Split(msinttypesPath + '/include'))

   # Add the Windows-specific libraries (already in main path)
   extLibs.extend(Split('ws2_32 winmm rpcrt4 shell32 ole32'))
elif buildTarget == 'win32.32bit':
   # Add the RTI
   if enableRTI == 'yes':
      addExternal(rtiPath, '/include/1.3', '/lib/winnt_vc++-10.0',
                  'librti13 libfedtime13 rtis mcast snpr parser')

   # Add libxml2 (and its dependency, iconv)
   if enableXML == 'yes':
      addExternal(xmlPath, '/include', '/lib', 'libxml2')
      addExternal(iconvPath, '/include', '/lib', 'iconv')

   # Add msinttypes headers
   extIncPath.extend(Split(msinttypesPath + '/include'))

   # Add the Windows-specific libraries (already in main path)
   extLibs.extend(Split('ws2_32 winmm rpcrt4 shell32 ole32'))
elif buildTarget == 'posix.64bit':
   # Add the RTI
   if enableRTI == 'yes':
      addExternal(rtiPath, '/include/1.3', '/lib/x86_64_g++-4.4',
                  'rti13 fedtime13 rtis msgflow mcast snpr parser')

   # Add the uuid library
   if enableUUID == 'yes':
      addExternal(uuidPath, '/include/ossp', '/lib64', 'ossp-uuid')

   # Add libxml2
   if enableXML == 'yes':
      addExternal(xmlPath, '/include/libxml2', '/lib64', 'xml2')

   # Add bluetooth
   if enableBluetooth == 'yes':
      addExternal(bluetoothPath, '/include', '/lib64', 'bluetooth')
elif buildTarget == 'posix.32bit':
  # Add the RTI
   if enableRTI == 'yes':
      addExternal(rtiPath, '/include/1.3', '/lib/linux_g++-4.4',
                  'rti13 fedtime13 rtis msgflow mcast snpr parser')

   # Add the uuid library
   if enableUUID == 'yes':
      addExternal(uuidPath, '/include', '/lib', 'uuid')

   # Add libxml2
   if enableXML == 'yes':
      addExternal(xmlPath, '/include/libxml2', '/lib', 'xml2')

   # Add bluetooth
   if enableBluetooth == 'yes':
      addExternal(bluetoothPath, '/include', '/lib', 'bluetooth')
elif buildTarget == 'ios':
   # Add the uuid library
   if enableUUID == 'yes':
      addExternal(uuidPath, '/include', '/lib', 'uuid')

   # Add libxml2
   if enableXML == 'yes':
      addExternal(xmlPath, '/include/libxml2', '/lib', 'xml2')
elif buildTarget == 'android':
   # Point to the NDK
   extIncPath.extend(Split(ndkPath + 
                           '/platforms/android-14/arch-arm/usr/include'))
   extLibPath.extend(Split(ndkPath + 
                           '/platforms/android-14/arch-arm/usr/lib'))

   # Point to library supporting RTTI
   extLibPath.extend(Split(ndkPath + 
                           '/sources/cxx-stl/gnu-libstdc++/libs/armeabi'))
   extLibs.extend(Split('gnustl_shared'))

   # Add the uuid library
   if enableUUID == 'yes':
      addExternal(uuidPath, '/include', '/lib', 'uuid')

   # Add libxml2
   if enableXML == 'yes':
      addExternal(xmlPath, '/include/libxml2', '/lib', 'xml2')

   # Add glob
   addExternal(globPath, '/include', '/libs/armeabi', 'glob')
else:
   # Unsupported architecture so bail
   print "Unsupported target type ", buildTarget
   sys.exit(0)

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


# Now, compile the shared and static libraries for ATLAS
atlasLib = basisEnv.StaticLibrary('atlas', source = atlasSource)
if buildTarget != 'ios':
   atlasDSO = basisEnv.SharedLibrary('atlas', source = atlasSource)


# Under Windows, embed the manifest into the .dll
if buildTarget == 'win32.64bit':
   embedManifest(basisEnv, atlasLib, 2)
   embedManifest(basisEnv, atlasDSO, 2)
elif buildTarget == 'win32.32bit':
   embedManifest(basisEnv, atlasLib, 2)
   embedManifest(basisEnv, atlasDSO, 2)

