

const path = require('path');
const root = path.join(__dirname, );

const androidPath = path.join(root, 'npm/android');

const winPath = path.join(root, 'npm/windows');
const vcxprojPath = path.join(root, 'npm/windows/prism.vcxproj');


const iosPath = path.join(root, 'npm/ios');

module.exports = {
    dependency: {
        platforms: {
            android: {
                sourceDir: root,
                cxxModuleCMakeListsPath: `CMakeLists.txt`, // This is relative to the sourceDir.
                cxxModuleCMakeListsModuleName: 'prism', // This is the name of the CMake target.
                //cxxModuleHeaderName: 'MyHeader', // CLI will include this header while linking.
            }
        },
    },
};
