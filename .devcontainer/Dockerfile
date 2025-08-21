FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.22.2"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends lcov clang-format clangd curl

COPY ./reinstall-git-extensions.sh /tmp/

RUN chmod +x /tmp/reinstall-git-extensions.sh && /tmp/reinstall-git-extensions.sh \
    && rm -f /tmp/reinstall-git-extensions.sh

# Set container user to UID 502 (with username 'vscode')
ARG USERNAME=vscode
ARG USER_UID=502
ARG USER_GID=502

RUN if ! getent group vscode; then \
      groupadd --gid 502 vscode; \
    else \
      groupmod -g 502 vscode; \
    fi \
    && if id -u vscode >/dev/null 2>&1; then \
      usermod -u 502 -g 502 vscode; \
    else \
      useradd --uid 502 --gid 502 -m vscode; \
    fi \
    && apt-get update && apt-get install -y sudo \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Set vscode user to uid=502,gid=20 (staff)
RUN groupadd -g 20 staff || true \
    && usermod -u 502 -g 20 vscode

RUN chown -R vscode:vscode /home/vscode

# Set the default user
USER $USERNAME
