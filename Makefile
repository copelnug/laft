# TODO:
#	> Version management
#	> Extract specific configuration
#	> Run tests
#	> Refactor source_for and tests_for so they use common code.
#	> Library name should be a variable

LIBS=laft-text laft-core
EXE_TEST=unittests
EXE_PERF=perftest

# Configuration
CXX=g++
CXXFLAGS=-fPIC -Wall -Wextra -Wconversion -std=c++1y
LDFLAGS=
LD=$(CXX)

# Destination directories
DIR_LIB=lib

# Intermediate directories
DIR_OBJECT=object/lib
DIR_OBJECT_TESTS=object/tests
DIR_OBJECT_PERF=object/perf
DIR_DEPENDANCIES=d/lib
DIR_DEPENDANCIES_TESTS=d/tests
DIR_DEPENDANCIES_PERF=d/perf

# Source directory
DIR_TESTS=tests
DIR_PERF=perf
DIR_SRC=src
DIR_INCLUDE=include

# Phony targets
.PHONY: all libs clean reset run-tests run-perf profile
all: libs $(EXE_TEST)
libs: $(addsuffix .so,$(addprefix $(DIR_LIB)/liblaft-,$(notdir $(wildcard $(DIR_SRC)/*))))
run-test: $(EXE_TEST)
	@export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(DIR_LIB); ./$(EXE_TEST)
run-perf: $(EXE_PERF)
	@export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(DIR_LIB); ./$(EXE_PERF)
profile: $(EXE_PERF)
	@export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(DIR_LIB); valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes ./$(EXE_PERF)

clean:
	@echo Removing objects files
	@rm -rf $(DIR_OBJECT)
	@echo Removing dependancies files
	@rm -rf $(DIR_DEPENDANCIES)
	@echo Removing test objects files
	@rm -rf $(DIR_OBJECT_TESTS)
	@echo Removing test dependancies files
	@rm -rf $(DIR_DEPENDANCIES_TESTS)
	@echo Removing performance test objects files
	@rm -rf $(DIR_OBJECT_PERF)
	@echo Removing performance test dependancies files
	@rm -rf $(DIR_DEPENDANCIES_PERF)
reset: clean
	@echo Removing performance test
	@rm -f $(EXE_PERF)
	@echo Removing test binary
	@rm -f $(EXE_TEST)
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
$(EXE_TEST): $(patsubst $(DIR_TESTS)/%.cpp, $(DIR_OBJECT_TESTS)/%.o, $(wildcard $(DIR_TESTS)/*/*.cpp) $(wildcard $(DIR_TESTS)/*.cpp)) $(patsubst %, $(DIR_LIB)/lib%.so, $(LIBS))
	@echo "Build test: $@"
	@$(LD) -o $@ $(LDFLAGS) $(filter %.o,$^) -L$(DIR_LIB) $(addprefix -l, $(LIBS))

$(DIR_OBJECT_TESTS)/%.o: $(DIR_TESTS)/%.cpp
	@echo "\tCompile: $<"
	@mkdir -p $(dir $@)
	@$(CXX) -o $@ -c $< $(CXXFLAGS) -I $(DIR_INCLUDE)
	@mkdir -p $(dir $(patsubst $(DIR_TESTS)/%.cpp,$(DIR_DEPENDANCIES_TESTS)/%.d,$<))
	@$(CXX) -MM $< -o $(patsubst $(DIR_TESTS)/%.cpp,$(DIR_DEPENDANCIES_TESTS)/%.d,$<) -MT $@ -MP -I $(DIR_INCLUDE) -I $(DIR_TESTS)

# Performance
$(EXE_PERF): $(patsubst $(DIR_PERF)/%.cpp, $(DIR_OBJECT_PERF)/%.o, $(wildcard $(DIR_PERF)/*/*.cpp) $(wildcard $(DIR_PERF)/*.cpp)) $(patsubst %, $(DIR_LIB)/lib%.so, $(LIBS))
	@echo "Build performance test: $@"
	@$(LD) -o $@ $(LDFLAGS) $(filter %.o,$^) -L$(DIR_LIB) $(addprefix -l, $(LIBS))

$(DIR_OBJECT_PERF)/%.o: $(DIR_PERF)/%.cpp
	@echo "\tCompile: $<"
	@mkdir -p $(dir $@)
	@$(CXX) -o $@ -c $< $(CXXFLAGS) -I $(DIR_INCLUDE)
	@mkdir -p $(dir $(patsubst $(DIR_PERF)/%.cpp,$(DIR_DEPENDANCIES_PERF)/%.d,$<))
	@$(CXX) -MM $< -o $(patsubst $(DIR_PERF)/%.cpp,$(DIR_DEPENDANCIES_PERF)/%.d,$<) -MT $@ -MP -I $(DIR_INCLUDE) -I $(DIR_TESTS)
	
# Specific
$(DIR_LIB)/liblaft-core.so: $(call source_for, core)
$(DIR_LIB)/liblaft-text.so: $(call source_for, text)
$(DIR_LIB)/liblaft-math-stats.so: $(call source_for, math-stats)

# Include
-include $(DIR_DEPENDANCIES)/*/*.d
-include $(DIR_DEPENDANCIES)/*/*/*.d
-include $(DIR_DEPENDANCIES_TESTS)/*.d
-include $(DIR_DEPENDANCIES_TESTS)/*/*.d
-include $(DIR_DEPENDANCIES_TESTS)/*/*/*.d
-include $(DIR_DEPENDANCIES_PERF)/*.d

