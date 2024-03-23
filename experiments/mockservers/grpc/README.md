# Get
grpcurl -import-path ./protocols -proto match.proto -plaintext -d @ localhost:8080 protocols.MatchService/Get <<EOM
{
"req": "oi"
}
EOM

# Live
grpcurl -import-path ./protocols -proto match.proto -plaintext -d @ localhost:8080 protocols.MatchService/Live <<EOM
{
"req": "oi"
}
EOM
