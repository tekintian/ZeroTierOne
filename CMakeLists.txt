cmake_minimum_required(VERSION 3.8...3.27)

project(
  ZeroTierOne
  VERSION 1.13.0
  LANGUAGES CXX C OBJC
)

set(CORE_OBJS
  node/AES.cpp
  node/AES_aesni.cpp
  node/AES_armcrypto.cpp
  node/C25519.cpp
  node/Capability.cpp
  node/CertificateOfMembership.cpp
  node/CertificateOfOwnership.cpp
  node/Identity.cpp
  node/IncomingPacket.cpp
  node/InetAddress.cpp
  node/Membership.cpp
  node/Metrics.cpp
  node/Multicaster.cpp
  node/Network.cpp
  node/NetworkConfig.cpp
  node/Node.cpp
  node/OutboundMulticast.cpp
  node/Packet.cpp
  node/Path.cpp
  node/Peer.cpp
  node/Poly1305.cpp
  node/Revocation.cpp
  node/Salsa20.cpp
  node/SelfAwareness.cpp
  node/SHA512.cpp
  node/Switch.cpp
  node/Tag.cpp
  node/Topology.cpp
  node/Trace.cpp
  node/Utils.cpp
  node/Bond.cpp
)

set(ONE_OBJS
  service/SoftwareUpdater.cpp
  service/OneService.cpp

  controller/EmbeddedNetworkController.cpp
  controller/DBMirrorSet.cpp
  controller/DB.cpp
  controller/FileDB.cpp
  controller/LFDB.cpp
  controller/PostgreSQL.cpp

  osdep/EthernetTap.cpp
  osdep/ManagedRoute.cpp
  osdep/Http.cpp
  osdep/OSUtils.cpp
  osdep/PortMapper.cpp
  osdep/MacEthernetTap.cpp
  osdep/MacKextEthernetTap.cpp
  osdep/MacDNSHelper.mm

  ext/http-parser/http_parser.c
)

# prometheus
add_subdirectory("ext/prometheus-cpp-lite-1.0/core")
add_subdirectory("ext/prometheus-cpp-lite-1.0/simpleapi")
add_subdirectory("ext/prometheus-cpp-lite-1.0/3rdpatry/http-client-lite")
target_compile_features(prometheus-cpp-simpleapi PUBLIC cxx_std_11)

# selftest
add_executable(selftest selftest.cpp
  ${CORE_OBJS}
  osdep/OSUtils.cpp
)

target_compile_features(selftest PUBLIC cxx_std_17)
target_include_directories(selftest PUBLIC ext include)
target_link_libraries(selftest prometheus-cpp-simpleapi)
set_target_properties(selftest PROPERTIES OUTPUT_NAME "zerotier-selftest")



# zerotier-one
add_executable(zerotier-one
  one.cpp
  ${CORE_OBJS}
  ${ONE_OBJS}
)

target_compile_features(zerotier-one PUBLIC cxx_std_17)
target_include_directories(zerotier-one PUBLIC ext include)
target_link_libraries(zerotier-one prometheus-cpp-simpleapi)
target_link_libraries(zerotier-one stdc++ "-framework CoreFoundation" "-framework CoreServices" "-framework SystemConfiguration" "-framework Security" objc)

target_compile_definitions(zerotier-one PRIVATE
    $<$<CONFIG:Debug>:
        ZT_DEBUG=1
        ZT_TRACE=1
    >
    # ZT_ARCH_ARM_HAS_NEON=1
)
