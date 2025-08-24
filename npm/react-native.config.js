// react-native.config.js
const path = require('path');

const dirname = path.resolve(__dirname, );
console.log('1111111111111111 dirname is:', dirname);

module.exports = {
  dependency: {
    platforms: {
      android: {
        sourceDir: './android',
      },
    },
  },
};
