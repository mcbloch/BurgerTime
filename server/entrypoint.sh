#!/bin/sh

exec python -m uvicorn --host 0.0.0.0 main:app
