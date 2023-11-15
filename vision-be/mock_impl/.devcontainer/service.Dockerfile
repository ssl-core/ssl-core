FROM robocin/robocin-ines:manual-robocup-v4

RUN mkdir build && cd build && cmake .. && make -j6

CMD ["../bin/client 0"]