curl -OL https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0-win32.zip -o glew-2.1.0-win32.zip
7z x glew-2.1.0-win32.zip
set GLEW_ROOT=%CD%\glew-2.1.0

curl -OL https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip -o glm-0.9.9.3.zip
7z x glm-0.9.9.3.zip
set GLM_ROOT=%CD%\glm
