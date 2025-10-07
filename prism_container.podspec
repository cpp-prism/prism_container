
Pod::Spec.new do |s|
  s.name         = "prism_container"
  s.version      = "1.0.0"
  s.summary      = "c++ static DI library"
  s.homepage     = "https://github.com/cpp-prism/prism_container"
  s.license      = "MIT"
  s.authors      = "hbb"

  s.ios.deployment_target = '12.0'
  s.osx.deployment_target = '10.15'

  s.source_files = "include/**/*.{cpp,c,h,hpp}" , "src/**/*.{cpp,c,mm,m}"
  s.public_header_files = "include/**/*.{cpp,c,h,hpp}"
  s.private_header_files = "./**/*.{cpp,c,h,hpp}"
  s.header_mappings_dir = "include"

  s.source       = { :git => "https://github.com/cpp-prism/prism_container.git" }
  root = File.expand_path(__dir__)



  s.pod_target_xcconfig = {
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'CLANG_CXX_LIBRARY' => 'libc++',
    'OTHER_CPLUSPLUSFLAGS' => '-DUSING_COCOAPODS=1 -DPRISM_CONTIANER_LIBRARY=1'
  }


end
