# OBS Shared Memory Image Source Plugin

This repository was generated from the OBS Plugin Template repository.

## Functionality

This plugin creates a **Shared Memory Image** source type in OBS Studio, which uses the Windows-only ```gs_texture_open_shared()``` OBS graphics function to display an image (```ID3D11Texture2D```) already stored in the GPU.

## Configuration

This plugin currently has no Inter-Process Communication (IPC) options for configuration, so the ```HANDLE``` and ```IDXGIKeyedMutex``` need manual configuration in the source's properties.

The *Shared Memory Handle* setting is a string, and the C ```strtoll()``` function is used to convert it to a ```uint32_t```. The call to ```strtoll()``` does not assume a base type, so a base 10 integer can be entered as can octal (prepend with ```0```) or hexadecimal (prepend with ```0x```).

## Notes

The ```video_render``` callback (```shm_image_source_render()```) will only wait 16 milliseconds for a mutex lock. If it can't obtain a mutex lock then the plugin doesn't draw anything for that frame. ***This can potentially cause flickering.***

This plugin was created as part of [**Issue #1 (Use DXGI) in watfordjc/csharp-message-to-image-library**](https://github.com/watfordjc/csharp-message-to-image-library/issues/1). Message to Image Library was itself created as part of [**Issue #36 (Add Tweet Sync Support) in watfordjc/csharp-stream-controller**](https://github.com/watfordjc/csharp-stream-controller/issues/36).

This plugin is essentially a wrapper around OBS function ```gs_texture_open_shared()``` and should work with any ```ID3D11Texture2D``` created with ```MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX``` and ```BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE```.

## Additional Disclaimer

**This plugin is pre-alpha, still in development, and has undergone very little testing.**

---

<details>
<summary>
Collapsible original README text (will be deleted in a later commit)
</summary>

## Introduction

This plugin is meant to make it easy to quickstart development of new OBS plugins. It includes:

- The CMake project file
- Boilerplate plugin source code
- A continuous-integration configuration for automated builds (a.k.a Build Bot)

## Configuring

Open `CMakeLists.txt` and edit the following lines at the beginning:

```cmake
# Change `obs-plugintemplate` to your plugin's name in a machine-readable format
# (e.g.: obs-myawesomeplugin) and set the value next to `VERSION` as your plugin's current version
project(obs-plugintemplate VERSION 1.0.0)

# Replace `Your Name Here` with the name (yours or your organization's) you want
# to see as the author of the plugin (in the plugin's metadata itself and in the installers)
set(PLUGIN_AUTHOR "Your Name Here")

# Replace `com.example.obs-plugin-template` with a unique Bundle ID for macOS releases
# (used both in the installer and when submitting the installer for notarization)
set(MACOS_BUNDLEID "com.example.obs-plugintemplate")

# Replace `me@contoso.com` with the maintainer email address you want to put in Linux packages
set(LINUX_MAINTAINER_EMAIL "me@contoso.com")
```

## CI / Build Bot

The CI scripts are made for Azure Pipelines. The sections below detail some of the common tasks possible with that CI configuration.

### Retrieving build artifacts

Each build produces installers and packages that you can use for testing and releases. These artifacts can be found a Build's page on Azure Pipelines.

#### Building a Release

Simply create and push a tag, and Azure Pipelines will run the pipeline in Release Mode. This mode uses the tag as its version number instead of the git ref in normal mode.

### Signing and Notarizing on macOS

On macOS, Release Mode builds will be signed and sent to Apple for notarization if `macosSignAndNotarize` is set to `True` at the top of the `azure-pipelines.yml` file. **You'll need a paid Apple Developer Account for this.**

In addition to enabling `macosSignAndNotarize`, you'll need to setup a few more things for Signing and Notarizing to work:

- On your Apple Developer dashboard, go to "Certificates, IDs & Profiles" and create two signing certificates:
    - One of the "Developer ID Application" type. It will be used to sign the plugin's binaries
    - One of the "Developer ID Installer" type. It will be used to sign the plugin's installer
- Using the Keychain app on macOS, export these two certificates and keys into a .p12 file **protected with a strong password**
- Add that `Certificates.P12` file as a [Secure File in Azure Pipelines](https://docs.microsoft.com/en-us/azure/devops/pipelines/library/secure-files?view=azure-devops) and make sure it is named `Certificates.p12`
- Add the following secrets in your pipeline settings:
    - `secrets.macOS.certificatesImportPassword`: Password of the .p12 file generated earlier
    - `secrets.macOS.codeSigningIdentity`: Name of the "Developer ID Application" signing certificate generated earlier
    - `secrets.macOS.installerSigningIdentity`: Name of "Developer ID Installer" signing certificate generated earlier
    - `secrets.macOS.notarization.username`: Your Apple Developer Account's username
    - `secrets.macOS.notarization.password`: Your Apple Developer Account's password
    - `secrets.macOS.notarization.providerShortName`: Identifier (`Provider Short Name`, as Apple calls it) of the Developer Team to which the signing certificates belong. 

</details>