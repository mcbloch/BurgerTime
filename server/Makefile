static/index.html: README.md
	pandoc README.md -o static/index.html

run: static/index.html
	python -m poetry run python -m uvicorn main:app --reload
