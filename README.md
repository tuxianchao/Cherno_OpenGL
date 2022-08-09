# Cherno_OpenGL


学习OpenGL练习代码


youtube: https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

opengl参考文档: http://docs.gl


glfw 和 glew 使用静态链接的方式,下载原工程后生成静态链接库，然后把静态链接库和头文件拷贝到工程中使用

## GLFW

- home page: https://www.glfw.org/
- github： https://github.com/glfw/glfw

### 生成 glfw

```
cd Dep_Source/glfw

cmake -S . -B Build -D BUILD_SHARED_LIBS=OFF -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF

cd Build

cmake --build . --config Debug

cmake --build . --config Release


```

## GLEW

- home page: http://glew.sourceforge.net/
- github: https://github.com/nigels-com/glew

使用 visual studio 解压打开 vc 项目生成后拷贝头文件和静态库
