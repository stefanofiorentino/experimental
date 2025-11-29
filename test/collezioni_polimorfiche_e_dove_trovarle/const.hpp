namespace {
constexpr auto EXPECTED_STRING_SHORT = R"(<document>
<on_off_light>
<is_on>
true
</is_on>
</on_off_light>
</document>
)";
constexpr auto EXPECTED_STRING_FULL = R"(<document>
<on_off_light>
<is_on>
true
</is_on>
</on_off_light>
<dimmable_light>
<is_on>
true
</is_on>
<level>
42
</level>
</dimmable_light>
</document>
)";
}