fn main() {
    tonic_build::configure()
        .out_dir("bin")
        .compile(&["src/proto/service.proto"], &["proto"])
        .unwrap();
}
