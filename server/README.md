# Burgertime server

Build using [FastAPI](https://fastapi.tiangolo.com).

Saves highscores in a central location.

## Development

Running the dev server

        poetry run python -m uvicorn main:app --reload

Or in docker

        docker build -t bt-server .
        docker run -it --rm -p 8000:8000 bt-server