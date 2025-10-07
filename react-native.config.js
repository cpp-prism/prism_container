

const path = require('path');
const root = path.join(__dirname, );


module.exports = {
    dependency: {
        platforms: {
            android: {
                sourceDir: root,
                cxxModuleCMakeListsPath: `CMakeLists.txt`, // This is relative to the sourceDir.
                cxxModuleCMakeListsModuleName: 'prism_container', // This is the name of the CMake target.
                //cxxModuleHeaderName: 'MyHeader', // CLI will include this header while linking.
            }
        },
    },
};
