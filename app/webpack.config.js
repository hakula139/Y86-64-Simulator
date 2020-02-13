const path = require('path');

const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const OptimizeCSSAssetsPlugin = require('optimize-css-assets-webpack-plugin');
const TerserPlugin = require('terser-webpack-plugin');

const rootPath = path.resolve(__dirname);
const srcPath = path.resolve(rootPath, 'src');
const distPath = path.resolve(rootPath, 'dist');

module.exports = {
  entry: {
    index: path.resolve(srcPath, 'index.js'),
    output: path.resolve(srcPath, 'output.js'),
    upload: path.resolve(srcPath, 'upload.js'),
  },
  output: {
    path: distPath,
    filename: 'js/[name].js',
    publicPath: '/assets/'
  },
  node: {
    fs: 'empty',
    net: 'empty'
  },
  plugins: [
    new HtmlWebpackPlugin({
      inject: true | 'body',
      hash: true,
      template: './src/index.html',
      favicon: './favicon/favicon.ico',
      filename: 'index.html'
    }),
    new MiniCssExtractPlugin({
      filename: 'css/[name].min.css',
      chunkFilename: 'css/[id].min.css',
      ignoreOrder: false
    })
  ],
  optimization: {
    minimize: true,
    minimizer: [
      new OptimizeCSSAssetsPlugin(),
      new TerserPlugin()
    ],
    splitChunks: {
      chunks: 'all'
    }
  },
  module: {
    rules: [
      {
        test: /\.css$/,
        use: [
          {
            loader: MiniCssExtractPlugin.loader
          },
          'css-loader'
        ],
      },
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader'
        }
      },
      {
        test: /\.(eot|woff2?|ttf)$/,
        use: {
          loader: 'url-loader',
          options: {
            name: 'font/[name].[ext]',
            limit: 4096
          }
        }
      },
      {
        test: /\.(ico)$/,
        use: {
          loader: 'file-loader?name=[name].[ext]'
        }
      },
      {
        test: /\.(png|jpe?g|gif|svg)$/,
        use: {
          loader: 'url-loader',
          options: {
            name: 'img/[name].[ext]',
            limit: 10240
          }
        }
      }
    ]
  },
  devServer: {
    historyApiFallback: true,
    hot: true,
    inline: true,
    progress: true,
  }
};
