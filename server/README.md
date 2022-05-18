# Burgertime server

Git repository: <https://github.com/mcbloch/burgertime>

Build using [FastAPI](https://fastapi.tiangolo.com).

Accompanying server for the Burgertime recreation found in the parent folder.

Saves highscores in a central location.

## API Usage

Detailed docs can be found at [/docs](./docs) or [/redoc](./redoc).

- Get all highscores: `http GET localhost:8000/highscores/`
- Create a new highscore: `http POST localhost:8000/highscores/ name=hello score=23`

## Development

Running the dev server

        poetry run python -m uvicorn main:app --reload

Or in docker

        docker build -t bt-server .
        docker run -it --rm -p 8000:8000 bt-server

## Roadmap

- Authentication using a shared secret
