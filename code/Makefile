.PHONY : all clean

EXECUTABLES := noexcept array constexpr_class namespace_class \
	multiple_inheritance \
	typeinfo \
	RTTI \
	class_member_pointer \
	function_table \
	union_class \
	local_class \
	union \
	use_sales_data \
	StrVec \
	StrBlob \
	function_call_operator \
	conversion_operator \
	manipulator \
	stringstream \
	seq_container \
	use_string \
	use_container_adaptor \
	overview_of_algorithms \
	lambda \
	bind \
	iterator \
	word_count \
	pair \
	associative_container \
	rvalue_reference \
	explict_cast \
	new_align \
	tuple \
	bitset \
	regex \
	random \

OBJS := ShowBackStageCls

BUILD_PATH = build

all : \
	$(foreach v, $(EXECUTABLES), $(BUILD_PATH)/$(v).exe) \
	$(foreach v, $(OBJS), $(BUILD_PATH)/$(v).o) \
	$(BUILD_PATH)/use_ShowBackStageCls.exe

CXX := g++
CXXFLAGS += -std=c++11 -Wall

$(BUILD_PATH) :
	@mkdir $(BUILD_PATH)

define COMPILE_template
  $$(BUILD_PATH)/$(1).exe : $(1).cc | $$(BUILD_PATH)
	$$(CXX) $$(CXXFLAGS) $$^ -o $$@
endef

$(foreach v, $(EXECUTABLES), $(eval $(call COMPILE_template,$(v))))

define COMPILE_obj_template
$$(BUILD_PATH)/$(1).o : $(1).cc | $$(BUILD_PATH)
	$$(CXX) $$(CXXFLAGS) -c $$^ -o $$(@)
endef

$(foreach v, $(OBJS), $(eval $(call COMPILE_obj_template,$(v))))

$(BUILD_PATH)/use_ShowBackStageCls.exe : use_ShowBackStageCls.cc $(BUILD_PATH)/ShowBackStageCls.o
	$(CXX) $(CXXFLAGS) $(^) -o $(@)

clean:
	rm -rf $(BUILD_PATH)
