const fs = require('fs');
const node_modules = fs.readdirSync('node_modules').filter(x => x !== '.bin');
//const HtmlWebpackPlugin = require('html-webpack-plugin');
//const ExtractTextPlugin = require('extract-text-webpack-plugin');
const globby = require('globby');
const webpack = require('webpack');

const shell = require('shelljs');




fs.writeFileSync('test/all.ts',
  globby.sync(['test/**/test_*.ts', 'test/**/*-test.tsx'])
    .map(file => file.replace('test/', '').replace(/\.tsx?$/, ''))
    .map(file => `import './${file}';`)
    .join('\n'));


function CreateIndex_d_ts() { }

CreateIndex_d_ts.prototype.apply = function(compiler) {

  compiler.plugin("emit", function(compilation, callback) {
    //chunk.files.forEach(function(filename) {
    //     console.log(compilation.assets[filename]);
    //});
    //console.log("The compilation is going to emit files...", compilation);
    shell.mkdir('-p', "dist/npm");
    shell.cp('../README.md', 'dist/npm/README.md');
    shell.cp('../LICENSE', 'dist/npm/LICENSE');
    shell.cp('package.json', 'dist/npm/package.json');
    shell.mkdir('-p', "dist/tsc");
    shell.exec("./node_modules/typescript/bin/tsc --outDir dist/tsc -d");
    shell.cp('dist/tsc/src/*.d.ts', "dist/npm");
    callback();
  });
};


module.exports = [
{
  target: 'node',
  entry: './src/index',
  output: {
    path: __dirname,
    filename: 'dist/npm/ipaddress.js',
    libraryTarget: 'commonjs2'
  },
  module: {
    loaders: [
      {
        test: /\.tsx?$/,
        loader: 'awesome-typescript?useBabel=true'
      }
    ]
  },
  externals: node_modules,
  devtool: 'source-map',
  resolve: {
    extensions: ['', '.ts', '.webpack.js', '.web.js', '.js']
  },
  plugins: [
    new CreateIndex_d_ts()
  ]
},{
  target: 'node',
  entry: './test/all',
  output: {
    path: __dirname + '/dist',
    filename: 'test.js',
    libraryTarget: 'commonjs2'
  },
  module: {
    loaders: [
      {
        test: /\.tsx?$/,
        loader: 'awesome-typescript?useBabel=true'
      },
      {
        test: /\.less$/,
        loader: 'style!css!less'
      }
    ]
  },
  externals: node_modules,
  devtool: 'source-map',
  resolve: {
    extensions: ['', '.tsx', '.ts', '.webpack.js', '.web.js', '.js']
  }
}];
