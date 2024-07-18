FROM rust:1.75

# Install build dependencies 
RUN apt-get update && apt-get install -y \
    protobuf-compiler \
    pkg-config \
    libssl-dev

# Set working directory
WORKDIR /app

# Copy source files 
COPY . .

# Build the project 
RUN cargo build --release

# Set the command to run your application
CMD ["./target/release/playback"]
