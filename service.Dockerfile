FROM robocin/robocin-ines:manual-robocup-v4

RUN mkdir /app

WORKDIR /app

COPY . .

WORKDIR /app/vision-be/mock_impl

RUN mkdir build && cd build && cmake .. && make -j6

CMD ["../bin/client"]