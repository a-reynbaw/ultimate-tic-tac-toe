FROM emscripten/emsdk

WORKDIR /app

COPY src/main_web.c ./src/
COPY src/state.c ./src/
COPY include/state.h ./src
COPY public/ ./public/

# Compile the C code.
RUN emcc src/*.c -o public/index.html --shell-file public/shell.html -sALLOW_MEMORY_GROWTH --post-js public/post.js

EXPOSE 8000

CMD ["python3", "-m", "http.server", "8000", "--directory", "public"]