mergeInto(LibraryManager.library, {
    jsSetGeyserInfo: function (type, count, data) {
        Module.updateWorld(type, count, data);
    },
});
