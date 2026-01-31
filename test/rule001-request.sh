curl \
    -H "Content-Type: application/json" \
    -d "@rule001-request.json" \
    -X POST \
    -m 5 \
    http://localhost:2346/api/v1/calculate-rule

