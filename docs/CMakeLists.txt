macro(add_sphinx_target TARGET_NAME BUILDER COMMENT_STR)
    add_custom_target(${TARGET_NAME}
    COMMAND sphinx-build -b ${BUILDER} . sphinx/build/${BUILDER}
        WORKING_DIRECTORY docs
        DEPENDS doxygen
        COMMENT ${COMMENT_STR}
    )

endmacro(add_sphinx_target)

add_custom_target(doxygen
    COMMAND doxygen Doxyfile
    COMMENT "Build doxygen xml files used by sphinx/breathe."
)

add_sphinx_target(docs-html
    html
    "Build html documentation"
)
