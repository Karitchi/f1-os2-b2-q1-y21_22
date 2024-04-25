FROM gcc:13.2.0
WORKDIR /app
COPY . .
RUN gcc -o main main.c
CMD ["./main"]