# Framecapper

Simple plugin that allows to force a 20/30 FPS framecap on PSVita apps by forcing specific VSync intervals.

## Changelog

### v.1.1

- Added support for injecting V-Sync into applications not using it.

### v.1.0

- Initial release.

## Differences between suprx files

- `Framecapper30.suprx`: 30 FPS cap for applications already having V-Sync functionality.
- `Framecapper20.suprx`: 20 FPS cap for applications already having V-Sync functionality.
- `Framecapper30Inject.suprx`: 30 FPS cap for applications not already having V-Sync functionality.
- `Framecapper20Inject.suprx`: 20 FPS cap for applications not already having V-Sync functionality.

## Setup Instructions

- Install this plugin by copying the suprx files (depending on what framerate target you want) to your taiHEN plugins folder (usually `ux0:tai`) and adding entries to your `config.txt` under `*ALL` or under specific Title ID sections you want to use the plugin on.
