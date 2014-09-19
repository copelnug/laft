# TODO:
#	> Version management
#	> Extract specific configuration
#	> Build options for library
#	> Run tests
#	> Refactor source_for and tests_for so they use common code.
#	> Library name should be a variable

# Configuration
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++1y
LDFLAGS=
LD=$(CXX)

# Destination directories
DIR_LIB=lib
DIR_EXE_TESTS=bin-tests

# Intermediate directories
DIR_OBJECT=object
DIR_OBJECT_TESTS=object-tests
DIR_DEPENDANCIES=d
DIR_DEPENDANCIES_TESTS=d-tests

# Source directory
DIR_TESTS=tests
DIR_SRC=src
DIR_INCLUDE=include

# Phony targets
.PHONY: all libs tests clean reset
all: libs
libs: $(addsuffix .so,$(addprefix $(DIR_LIB)/liblaft-,$(notdir $(wildcard $(DIR_SRC)/*))))
tests: $(addsuffix .tests,$(addprefix $(DIR_EXE_TESTS)/,$(notdir $(wildcard $(DIR_TESTS)/*))))
run-test: tests
	@

clean:
	@echo Removing objects files
	@rm -rf $(DIR_OBJECT)
	@echo Removing dependancies files
	@rm -rf $(DIR_DEPENDANCIES)
	@echo Removing test objects files
	@rm -rf $(DIR_OBJECT_TESTS)
	@echo Removing test dependancies files
	@rm -rf $(DIR_DEPENDANCIES_TESTS)
reset: clean
	@echo Removing test binaries
	@rm -rf $(DIR_EXE_TESTS)
	@echo Removing libraries
	@rm -rf $(DIR_LIB)

# Disable built in
.SUFFIXES:
# Prevent make from deleting .o file each time it compile
.SECONDARY:

# Functions
source_for=$(patsubst $(DIR_SRC)/%.cpp,$(DIR_OBJECT)/%.o,$(wildcard $(addsuffix /*.cpp, $(addprefix $(DIR_SRC)/,$(1)))))
tests_for=$(patsubst $(DIR_TESTS)/%.cpp,$(DIR_OBJECT_TESTS)/%.o,$(wildcard $(addsuffix /*.cpp, $(addprefix $(DIR_TESTS)/,$(1)))))

# Libs
$(DIR_LIB)/liblaft-%.so: $(DIR_SRC)/%
	@echo "Link: $<"
	@mkdir -p $(DIR_LIB)
	@$(LD) -o $@ -shared $(filter %.o,$^) -L$(DIR_LIB) $(patsubst $(DIR_LIB)/lib%.so,-l%,$(filter %.so,$^))

$(DIR_OBJECT)/%.o: $(DIR_SRC)/%.cpp
	@echo "\tCompile: $<"
	@mkdir -p $(dir $@)
	@$(CXX) -o $@ -c $< $(CXXFLAGS) -I $(DIR_INCLUDE)
	@mkdir -p $(dir $(patsubst $(DIR_SRC)/%.cpp,$(DIR_DEPENDANCIES)/%.d,$<))
	@$(CXX) -MM $< -o $(patsubst $(DIR_SRC)/%.cpp,$(DIR_DEPENDANCIES)/%.d,$<) -MT $@ -MP -I $(DIR_INCLUDE)

# Tests
$(DIR_EXE_TESTS)/%.tests: $(DIR_TESTS)/% $(DIR_LIB)/liblaft-%.so
	@echo "Build test: $@"
	@mkdir -p $(DIR_EXE_TESTS)
	@$(LD) -o $@ $(LDFLAGS) $(filter %.o,$^) -L$(DIR_LIB) $(patsubst $(DIR_LIB)/lib%.so,-l%,$(filter %.so,$^))

$(DIR_OBJECT_TESTS)/%.o: $(DIR_TESTS)/%.cpp
	@echo "\tCompile: $<"
	@mkdir -p $(dir $@)
	@$(CXX) -o $@ -c $< $(CXXFLAGS) -I $(DIR_INCLUDE)
	@mkdir -p $(dir $(patsubst $(DIR_TESTS)/%.cpp,$(DIR_DEPENDANCIES_TESTS)/%.d,$<))
	@$(CXX) -MM $< -o $(patsubst $(DIR_TESTS)/%.cpp,$(DIR_DEPENDANCIES_TESTS)/%.d,$<) -MT $@ -MP -I $(DIR_INCLUDE)

# Specific
$(DIR_LIB)/liblaft-core.so: $(call source_for, core)
$(DIR_LIB)/liblaft-math-stats.so: $(call source_for, math-stats)
$(DIR_EXE_TESTS)/core.tests: $(call tests_for,core)
$(DIR_EXE_TESTS)/math-stats.tests: $(call tests_for,math-stats)


# Include
-include $(DIR_DEPENDANCIES)/*/*.d
-include $(DIR_DEPENDANCIES)/*/*/*.d
-include $(DIR_DEPENDANCIES_TESTS)/*/*.d
-include $(DIR_DEPENDANCIES_TESTS)/*/*/*.d

