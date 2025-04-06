import path from "path";
import * as webpack from "webpack";
import HtmlWebpackPlugin from "html-webpack-plugin";
import CopyWebpackPlugin from "copy-webpack-plugin";

export default (
    _env: any,
    argv: webpack.WebpackOptionsNormalized
): webpack.Configuration => {
    var wasmpath = "out/build/wasm-release/src";
    if (argv.mode === "development") {
        wasmpath = "out/build/wasm-debug/src";
    }
    const config: webpack.Configuration = {
        entry: "./src/index.tsx",
        module: {
            rules: [
                {
                    test: /\.ts(x)?$/,
                    use: "ts-loader",
                    exclude: /node_modules/,
                },
                {
                    test: /\.css$/,
                    use: ["style-loader", "css-loader"],
                },
            ],
        },
        output: {
            path: path.resolve(__dirname, "out/html"),
            filename: "bundle-[contenthash:8].js",
        },
        resolve: {
            extensions: [".tsx", ".ts", ".js"],
        },
        performance: {
            hints: false,
        },
        plugins: [
            new HtmlWebpackPlugin({
                template: "./src/index.html",
                filename: "index.html",
                inject: "body",
                minify: {
                    collapseWhitespace: true,
                    removeComments: true,
                },
                hash: true,
            }),
            new CopyWebpackPlugin({
                patterns: [
                    { from: "oniWorldApp-*.js", context: wasmpath },
                    { from: "*-*.bin", context: wasmpath },
                    { from: "*", context: "public" },
                ],
            }),
        ],
    };
    return config;
};
