# OBS Blur Filter

<p align="center">
<image height="400px" src="/images/BlurFilterScreenshot.png">
</p>


## Introduction
This plugin is designed to allow you to set a simple blur filter on an image or video source in OBS.

This plugin is designed to be used with `OBS version 28` and above.

## Notes
This is a work in progress, the blur filter is simple and hasn't been optimised.

## Usage
To use this plugin:
- Right click on an image or video source and select `Filters`
- On the popup window that appears select the `+` button to add a new filter
- Select the filter called `Ashmanix Blur Filter`
- Use the dropdown to select between blur filter types
- Each filter will have its own properties you can set

## Build Instructions
To build this code use the included scripts that are taken from the [OBS Plugin Template](https://github.com/obsproject/obs-plugintemplate).

### GitHub Actions & CI
The scripts contained in github/scripts can be used to build and package the plugin and take care of setting up obs-studio as well as its own dependencies. A default workflow for GitHub Actions is also provided and will use these scripts.

#### Retrieving build artifacts
Each build produces installers and packages that you can use for testing and releases. These artifacts can be found on the action result page via the "Actions" tab in your GitHub repository.

#### Building a Release
Simply create and push a tag and GitHub Actions will run the pipeline in Release Mode. This mode uses the tag as its version number instead of the git ref in normal mode.
