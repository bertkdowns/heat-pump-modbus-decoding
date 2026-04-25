# Use a Python image with uv pre-installed
FROM python:3.10.0-slim


WORKDIR /app/

# We want to use UV to install everything globally, so we can run it with python directly later.
ENV UV_PROJECT_ENVIRONMENT=/usr/local 


# Copy only what is needed for deps installation
COPY pyproject.toml uv.lock /app/

# Install deps (expensive step), cached as long as:
#    - pyproject/lockfiles don't change
# Also mount uv binary temporarily and exclude from final image
RUN --mount=from=ghcr.io/astral-sh/uv:0.9.9,source=/uv,target=/bin/uv \
    uv sync --frozen --no-dev --no-cache --no-editable --no-install-project

# Copy the rest of the app
COPY . /app/

# Set command

CMD ["python", "electrical_mqtt.py"]

