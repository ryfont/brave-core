/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
 * This file manages the following:
 * - Lists of files needed to be translated (Which is all top level GRD and JSON files)
 * - All mappings for auto-generated Brave files from the associated Chromium files.
 * - Top level global string replacements, such as replacing Chromium with Brave
 */

const path = require('path')
const fs = require('fs')
const jsdom = require("jsdom");
const { JSDOM } = jsdom;

const srcDir = path.resolve(path.join(__dirname, '..', 'src'))

// chromium_strings.grd and any of its parts files that we track localization for in transifex
// These map to brave/app/resources/chromium_strings*.xtb
const chromiumStringsPath = path.resolve(path.join(srcDir, 'chrome', 'app', 'chromium_strings.grd'))
const braveStringsPath = path.resolve(path.join(srcDir, 'brave', 'app', 'brave_strings.grd'))
const chromiumSettingsPartPath = path.resolve(path.join(srcDir, 'chrome', 'app', 'settings_chromium_strings.grdp'))
const braveSettingsPartPath = path.resolve(path.join(srcDir, 'brave', 'app', 'settings_brave_strings.grdp'))

//Replace android strings.
const androidChromeStringsPath = path.resolve(path.join(srcDir, 'chrome', 'android', 'java', 'strings', 'android_chrome_strings.grd'))
const braveAndroidChromeStringsPath = path.resolve(path.join(srcDir, 'brave', 'android', 'java', 'strings', 'android_chrome_strings.grd'))

// component_chromium_strings.grd and any of its parts files that we track localization for in transifex
// These map to brave/app/strings/components_chromium_strings*.xtb
const chromiumComponentsChromiumStringsPath = path.resolve(path.join(srcDir, 'components', 'components_chromium_strings.grd'))
const braveComponentsBraveStringsPath = path.resolve(path.join(srcDir, 'brave', 'components', 'components_brave_strings.grd'))

// components/component_strings.grd and any of its parts files that we track localization for in transifex
// These map to brave/components/component_strings*.xtb
const chromiumComponentsStringsPath = path.resolve(path.join(srcDir, 'components', 'components_strings.grd'))
const braveComponentsStringsPath = path.resolve(path.join(srcDir, 'brave', 'components', 'components_strings.grd'))

// generated_resources.grd and any of its parts files that we track localization for in transifex
// There is also chromeos_strings.grdp, but we don't need to track it here because it is explicitly skipped in transifex.py
// These map to brave/app/resources/generated_resoruces*.xtb
const chromiumGeneratedResourcesPath = path.resolve(path.join(srcDir, 'chrome', 'app', 'generated_resources.grd'))
const braveGeneratedResourcesPath = path.resolve(path.join(srcDir, 'brave', 'app', 'generated_resources.grd'))
const chromiumGeneratedResourcesExcludes = new Set(["chromeos_strings.grdp"])

// The following are not generated files but still need to be tracked so they get sent to transifex
// These xtb files don't need to be copied anywhere.
// brave_generated_resources.grd maps to brave/app/resources/brave_generated_resources*.xtb,
// brave_components_strings.grd maps to brave/components/resources/strings/brave_components_resources*.xtb
// messages.json localization is handled inside of brave-extension.
const braveSpecificGeneratedResourcesPath = path.resolve(path.join(srcDir, 'brave', 'app', 'brave_generated_resources.grd'))
const braveResourcesComponentsStringsPath = path.resolve(path.join(srcDir, 'brave', 'components', 'resources', 'brave_components_strings.grd'))
const braveExtensionMessagesPath = path.resolve(path.join(srcDir, 'brave', 'components', 'brave_extension', 'extension', 'brave_extension', '_locales', 'en_US', 'messages.json'))
const braveRewardsExtensionMessagesPath = path.resolve(path.join(srcDir, 'brave', 'components', 'brave_rewards', 'resources', 'extension', 'brave_rewards', '_locales', 'en_US', 'messages.json'))

// Helper function to find all grdp parts in a grd.
function getGrdPartsFromGrd(path) {
  const grd = new JSDOM(fs.readFileSync(path, 'utf8'))
  const partTags = grd.window.document.getElementsByTagName("part")
  let parts = new Array()
  for (i = 0; i < partTags.length; i++) {
    parts.push(partTags[i].getAttribute('file'));
  }
  return parts
}

// Helper function to create a mapping for grd and all of its grdp parts.
function AddGrd(chromiumPath, bravePath, exclude = new Set()) {
  console.log("Adding mappings for GRD: " + chromiumPath)
  let mapping = {
    [chromiumPath]: bravePath
  }
  const grdps = getGrdPartsFromGrd(chromiumPath)
  if (grdps.length) {
    const chromiumDir = path.dirname(chromiumPath)
    const braveDir = path.dirname(bravePath)
    for (i = 0; i < grdps.length; i++) {
      if (exclude.has(grdps[i])) {
        continue
      }
      mapping[path.resolve(path.join(chromiumDir, grdps[i]))] = path.resolve(path.join(braveDir, grdps[i]))
    }
    console.log("  - Added " + (Object.keys(mapping).length - 1) + " GRDP.")
  }
  return mapping
}

// Add all GRD mappings here.
// Brave specific only grd and grdp files should NOT be added.
// When adding new grd or grdp files, never add a grdp part path without a parent grd path.
// Group them with a leading and trailing newline to keep this file organized.
// The first 3 are added explicitly because we change the file names.
const chromiumToAutoGeneratedBraveMapping = {
  [chromiumStringsPath]: braveStringsPath,
  [chromiumSettingsPartPath]: braveSettingsPartPath,

  [chromiumComponentsChromiumStringsPath]: braveComponentsBraveStringsPath,

  ...AddGrd(chromiumComponentsStringsPath, braveComponentsStringsPath),
  ...AddGrd(chromiumGeneratedResourcesPath, braveGeneratedResourcesPath, chromiumGeneratedResourcesExcludes),
  ...AddGrd(androidChromeStringsPath, braveAndroidChromeStringsPath)
}

// Same as with chromiumToAutoGeneratedBraveMapping but maps in the opposite direction
module.exports.autoGeneratedBraveToChromiumMapping = Object.keys(chromiumToAutoGeneratedBraveMapping)
    .reduce((obj, key) => ({ ...obj, [chromiumToAutoGeneratedBraveMapping[key]]: key }), {})

// All paths which are not generated
module.exports.braveNonGeneratedPaths = [
  braveSpecificGeneratedResourcesPath, braveResourcesComponentsStringsPath, braveExtensionMessagesPath, braveRewardsExtensionMessagesPath
]

// All paths which are generated
module.exports.braveAutoGeneratedPaths = Object.values(chromiumToAutoGeneratedBraveMapping)

// Brave specific strings and Chromium mapped Brave strings will be here.
// But you only need to add the Brave specific strings manually here.
module.exports.allBravePaths = module.exports.braveNonGeneratedPaths.concat(module.exports.braveAutoGeneratedPaths)

// Get all GRD and JSON paths whether they are generatd or not
// Push and pull scripts for l10n use this.
// Transifex manages files per grd and not per grd or grdp.
// This is because only 1 xtb is created per grd per locale even if it has multiple grdp files.
module.exports.braveTopLevelPaths = module.exports.allBravePaths.filter((x) => ['grd', 'json'].includes(x.split('.').pop()))

// ethereum-remote-client path relative to the Brave paths
module.exports.ethereumRemoteClientPaths = [
  '../../../ethereum-remote-client/app/_locales/en/messages.json',
  '../../../ethereum-remote-client/brave/app/_locales/en/messages.json'
]

// This simply reads Chromium files that are passed to it and replaces branding strings
// with Brave specific branding strings.
// Do not use this for filtering XML, instead use chromium-rebase-l10n.py.
// Only add idempotent replacements here (i.e. don't append replace A with AX here)
module.exports.rebaseBraveStringFilesOnChromiumL10nFiles = async function (path) {
  const ops = Object.entries(chromiumToAutoGeneratedBraveMapping).map(async ([sourcePath, destPath]) => {
    let contents = await new Promise(resolve => fs.readFile(sourcePath, 'utf8', (err, data) => resolve(data)))
    for (const replacement of defaultReplacements) {
      contents = contents.replace(replacement[0], replacement[1])
    }
    await new Promise(resolve => fs.writeFile(destPath, contents, 'utf8', resolve))
  })
  await Promise.all(ops)
}

// Straight-forward string replacement list.
// Consider mapping chromium resource ID to a new brave resource ID
// for whole-message replacements, instead of adding to this list.
const defaultReplacements = [
  [/Automatically send usage statistics and crash reports to Google/g, 'Automatically send crash reports to Google'],
  [/Automatically sends usage statistics and crash reports to Google/g, 'Automatically sends crash reports to Google'],
  [/Chrome Web Store/g, 'Web Store'],
  [/The Chromium Authors/g, 'Brave Software Inc'],
  [/Google Chrome/g, 'Brave'],
  [/Chromium/g, 'Brave'],
  [/Chrome/g, 'Brave'],
  [/Google/g, 'Brave'],
  [/You're incognito/g, 'This is a private window'],
  [/an incognito/g, 'a private'],
  [/an Incognito/g, 'a Private'],
  [/incognito/g, 'private'],
  [/Incognito/g, 'Private'],
  [/inco\&amp\;gnito/g, '&amp;private'],
  [/Inco\&amp\;gnito/g, '&amp;Private'],
  [/People/g, 'Profiles'],
  // 'people' but only in the context of profiles, not humans.
  [/(?<!authenticate )people(?! with slow connections?)/g, 'profiles'],
  [/(Person)(?!\w)/g, 'Profile'],
  [/(person)(?!\w)/g, 'profile'],
  [/Bookmarks Bar\n/g, 'Bookmarks\n'],
  [/Bookmarks bar\n/g, 'Bookmarks\n'],
  [/bookmarks bar\n/g, 'bookmarks\n'],
]
