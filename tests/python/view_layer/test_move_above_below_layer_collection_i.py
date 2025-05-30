# SPDX-FileCopyrightText: 2017-2022 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

# ############################################################
# Importing - Same For All Render Layer Tests
# ############################################################

import unittest

from view_layer_common import (
    MoveLayerCollectionTesting,
    setup_extra_arguments,
)


# ############################################################
# Testing
# ############################################################

class UnitTesting(MoveLayerCollectionTesting):
    def get_reference_scene_tree_map(self):
        # original tree, no changes
        return self.get_initial_scene_tree_map()

    def get_reference_layers_tree_map(self):
        reference_layers_map = [
            ['Layer 1', [
                'Master Collection',
                'C',
                '3',
            ]],
            ['Layer 2', [
                '3',
                'C',
                'dog',
                'cat',
            ]],
        ]
        return reference_layers_map

    def test_layer_collection_move_a(self):
        """
        Test outliner operations
        """
        self.setup_tree()
        self.assertTrue(self.move_below('Layer 2.C', 'Layer 2.3'))
        self.compare_tree_maps()

    def test_layer_collection_move_b(self):
        """
        Test outliner operations
        """
        self.setup_tree()

        # collection that will be moved
        collection_original = self.parse_move('Layer 2.C')
        collection_original.enabled = True
        collection_original.selectable = False

        # move
        self.assertTrue(self.move_below('Layer 2.C', 'Layer 2.3'))
        self.compare_tree_maps()

        # we expect the settings to be carried along from the
        # original layer collection
        collection_new = self.parse_move('Layer 2.C')
        self.assertEqual(collection_new.enabled, True)
        self.assertEqual(collection_new.selectable, False)


# ############################################################
# Main - Same For All Render Layer Tests
# ############################################################

if __name__ == '__main__':
    UnitTesting._extra_arguments = setup_extra_arguments(__file__)
    unittest.main()
