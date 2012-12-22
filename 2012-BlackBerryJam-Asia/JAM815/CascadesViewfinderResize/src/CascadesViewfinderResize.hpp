// Default empty project template
#ifndef CascadesViewfinderResize_HPP_
#define CascadesViewfinderResize_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class CascadesViewfinderResize : public QObject
{
    Q_OBJECT
public:
    CascadesViewfinderResize(bb::cascades::Application *app);
    virtual ~CascadesViewfinderResize() {}
};


#endif /* CascadesViewfinderResize_HPP_ */
