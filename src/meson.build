filesorter_sources = [
  'main.c',
  'filesorter-application.c',
  'filesorter-window.c',
  'sorting.c',
]

filesorter_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1', version: '>= 1.4'),
]

filesorter_sources += gnome.compile_resources('filesorter-resources',
  'filesorter.gresource.xml',
  c_name: 'filesorter'
)

executable('filesorter', filesorter_sources,
  dependencies: filesorter_deps,
       install: true,
)
