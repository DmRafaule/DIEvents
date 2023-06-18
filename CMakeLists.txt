cmake_minimum_required(VERSION 3.5)
project(DIEvents VERSION 0.1 DESCRIPTION "Event module" LANGUAGES CXX)


file( GLOB SOURCES **.cpp )

SetFlags()
SetLibraries( DIWindow )
CreateLibrary( ${PROJECT_NAME} )