file(REMOVE_RECURSE
  "Config-Test.xml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Config-Test.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
