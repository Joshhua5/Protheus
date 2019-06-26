{
  "targets": [
    {
      "target_name": "protheus",
      "sources": [
        "src/Editor.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'libraries': [],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [""],
      "cflags_cc!": [""],
      "defines": ["NAPI_CPP_EXCEPTIONS"]
    }
  ]
}