add_executable(FemBasic main2021.cpp ${SOURCES} ${HEADERS})
target_link_libraries (FemBasic Eigen3::Eigen Catch2::Catch2)


add_executable(TestIntegrate TestIntegrate.cpp TestGeom.cpp ${SOURCES} ${HEADERS})
target_link_libraries (TestIntegrate PRIVATE Eigen3::Eigen Catch2::Catch2)
