#!bin/bash

# Generates the proto definitions for the server and the client 
# client definitions are placed in the proto directory in the client code
# server definitions are placed in the chess-engine-proto directory under cpp-defs

set -euxo pipefail

echo "Generating type defs ..."

# protoc \
# 	--plugin="protoc-gen-ts=/home/swarajy/Documents/chess-engine/chess-engine-frontend/node_modules/.bin/protoc-gen-ts" \
# 	--ts_opt=esModuleInterop=true \
# 	--ts_out="/home/swarajy/Documents/chess-engine/chess-engine-frontend/src/proto" \
# 	"/home/swarajy/Documents/chess-engine/chess-engine-proto/cpp-defs/"

# generate cpp_defs
protoc  --proto_path=/home/swarajy/Documents/chess-engine/chess-engine-proto/proto/ \
        --cpp_out=/home/swarajy/Documents/chess-engine/chess-engine-proto/cpp-defs \
        /home/swarajy/Documents/chess-engine/chess-engine-proto/proto/chess_service.proto

# generate ts_defs
protoc \
    --proto_path=/home/swarajy/Documents/chess-engine/chess-engine-proto/proto/ \
	--plugin="protoc-gen-ts=/home/swarajy/Documents/chess-engine/chess-engine-frontend/node_modules/.bin/protoc-gen-ts" \
	--ts_opt=esModuleInterop=true \
	--ts_out="/home/swarajy/Documents/chess-engine/chess-engine-frontend/src/proto" \
    /home/swarajy/Documents/chess-engine/chess-engine-proto/proto/chess_service.proto
    
echo "Done..."