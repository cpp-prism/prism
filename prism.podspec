
Pod::Spec.new do |s|
  s.name         = "prism"
  s.version      = "1.0.0"
  s.summary      = "c++ static reflection header only library "
  s.homepage     = "https://github.com/cpp-prism/prism"
  s.license      = "MIT"
  s.authors      = "hbb"

  s.ios.deployment_target = '12.0'
  s.osx.deployment_target = '10.15'

  s.source_files = "include/**/*.{cpp,c,h,hpp}"
  s.public_header_files = "include/**/*.{cpp,c,h,hpp}"
  #s.private_header_files = "include/**/*.{cpp,c,h,hpp}"
  s.header_mappings_dir = "include"


  s.source       = { :git => "https://github.com/nocanstillbb/prism-rn.git" }
  root = File.expand_path(__dir__)


  #s.xcconfig = {
  #  'HEADER_SEARCH_PATHS' => [
  #    "\"#{root}/include\"",
  #    "\"#{root}/include/prism\"",
  #    "\"#{root}/include/prism/utilities\"",
  #  ].join(' ')
  #}

  s.pod_target_xcconfig = {
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'CLANG_CXX_LIBRARY' => 'libc++'
  }


end
