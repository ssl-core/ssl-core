# match-bff

## Folder Structure

- cmd/ é o diretório que aloca o arquivo main.go, que idealmente deve possuir apenas a função principal para invocar o código contido nos pacotes internal/ ou pkg/.

- internal/ é o diretório onde estarão definidos os pacotes do projeto que serão privados, ou seja, que não poderão ser importados como uma biblioteca por outras aplicações Go. Essa é uma regra imposta pelo próprio compilador.

- pkg/: é o diretório onde deixaremos expostos os pacotes que podem ser importados como uma biblioteca por outras aplicações Go. Nesse exemplo: os stubs gerados pelos arquivos Protobuf.

## Installation

curl -LO https://github.com/protocolbuffers/protobuf/releases/download/v25.3/protoc-25.3-linux-x86_64.zip
unzip protoc-25.3-linux-x86_64.zip -d $HOME/.local
export PATH="$PATH:$HOME/.local/bin"
go install google.golang.org/protobuf/cmd/protoc-gen-go@latest
go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest

## Test

curl -sSL "https://github.com/fullstorydev/grpcurl/releases/download/v1.8.9/grpcurl_1.8.9_linux_x86_64.tar.gz" | sudo tar -xz -C /usr/local/bin

grpcurl -import-path proto -proto proto/v1/match.proto -plaintext -d @ localhost:8080 v1.MatchService/Get <<EOM
{
"req": "oi"
}
EOM
